//=============================================================================
// Title: SPI/I2C block
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 3/16/2016
// Last modified: 7/22/2016 (AC)
// Description: core of the electronics that communicates with the ADC-DAC board
//              and with the DUT board
//=============================================================================

module spi_i2c_block #(
  parameter INPUT_CLK_FREQUENCY = 50000000,   // Input clock frequency
  parameter I2C_CLK_FREQUENCY = 50000,        // I2C clock frequency
  parameter SPI_CLK_FREQUENCY = 25000000      // SPI clock frequency
)
(
  // Clock interface
  input  CLK,                                 // System clock (50MHz)
  input  RST_N,                               // Active low reset
  
  output BUSY,                                // Indicates transaction in progress

  // Reg interface
  input  [3:0] REG_ADDR,                      // Reg address   
  input  [15:0] REG_IN,                       // Reg input data bus
  input  REG_WREN,                            // Reg write enable 
  output reg [15:0] REG_OUT,                  // Reg output data bus
  
  input  SCLK,                                // SPI Clock

  // FIFO interface              
  input  FIFO_FULL,                   
  output [31:0] FIFO_DATA_IN,
  output FIFO_WREN,              

  // Signals to/from SPI/I2C slaves
  
  // I2C interface to communicate with the digital potentiometers on the ADC-DAC board
  input  SCL_IN,           // From slave to master (FPGA)
  input  SDA_IN,           // From slave to master (FPGA)
  output SCL_DIR_N,        // 0 == output enabled -> Master (FPGA) to Slave direction
  output SCL_OUT,          // From master (FPGA) to slave
  output SDA_DIR_N,        // 0 == output enabled -> Master (FPGA) to Slave direction
  output SDA_OUT,          // From master (FPGA) to slave

  // ADC interface
  input  [31:0] ADC_DATA_OUT,  // Input data from ADC
  output [3:0]  ADC_CS_IN,     // ADC_CS_IN[0] -> ADC_DATA_OUT[7:0] ... ADC_CS_IN[3] -> ADC_DATA_OUT[31:24]
  
  // DAC interface
  output [7:0]  DAC_DATA_IN,
  output [7:0]  DAC_SYNC_IN,
  
  // Waveform generator interface
  // List of signals (DUT side <-> ADC-DAC board side)

  output [7:0]  WF_DATA_IN,
  output [7:0]  WF_FSYNC_IN,
  
  // Next signals are GPIO at the interface FPGA <-> ADC-DAC board
  inout  [15:0] GPIO_IN,   // General purpose data lines (bidir - direction is set via GPIO_DIR_N bus) 
  
  // Differential IOs
  input  [19:0] DIFF_IN,    // Inputs of the FPGA
  output [19:0] DIFF_OEN,   // Output enable of the FPGA
  output [19:0] DIFF_OUT    // Outputs of the FPGA

);

//========================================================================
//                          Internal Signals 
//========================================================================

wire [31:0]   data_in;
wire [1151:0] data_out;
wire [95:0]   data_adc_block_0; 
wire [95:0]   data_adc_block_1; 
wire [95:0]   data_adc_block_2; 
wire [95:0]   data_adc_block_3;
wire [255:0]  data_diffio_checker;
wire [7:0]    diffio_checker_errors;
wire send_data;
wire send_header;
wire send_trailer;
wire [47:0]   start_spi;
wire [47:0]   busy_spi;
wire [4:0]    num_of_bits_to_read;
wire [4:0]    num_of_bits_to_write;
wire [47:0]   SS_b;
wire [31:0]   start_adc;
wire [7:0]    start_dac;
wire          start_diffio_checker;
wire          start_interrupt_checker;
wire          start_dp;
wire [7:0]    start_wf;
wire          start_i2c_dut;
wire          start_i2c_dut_aux;
wire [31:0]   busy_adc;
wire [7:0]    busy_dac;
wire          busy_diffio_checker;
wire          busy_interrupt_checker;
wire          busy_dp;
wire [7:0]    busy_wf;
wire          busy_i2c_dut;
wire          busy_i2c_dut_aux;
wire [1:0]    num_bytes_to_rec;
wire [1:0]    num_bytes_to_transm;
wire [47:0] mosi;
wire [47:0] miso;
wire sclk_en;
wire interrupt_bar;
wire interrupt_init_state;
wire interrupt_transition;

wire sda_dir;

wire [23:0] data_i2c_dut, data_i2c_dut_aux;
wire [1:0] op_type_spi;
wire [247:0] data_out_i2c_dut, data_out_i2c_dut_aux;

wire [15:0] inreg_out;

reg cmd_wren, cmdaux_wren, datalow_wren, datahigh_wren, outreg_wren;
reg [15:0] cmd_out, cmdaux_out, datalow_out, datahigh_out, outreg_out;

//========================================================================
//                      Internal address space (registers) 
//========================================================================

// Internal address space - 4 bits wide
// 4'h1 - Input register
// 4'h2 - Command register
// 4'h3 - Data low register
// 4'h4 - Data high register
// 4'h5 - Output register - from FPGA to DUT (register which constains settable and readable bits)
// 4'h6 - Command auxiliary register
// any other value - Input register - from DUT to FPGA (read only)

always @(*) begin
  case (REG_ADDR)
    4'h1: begin
      cmd_wren      = 1'b0;
      cmdaux_wren   = 1'b0;
      datalow_wren  = 1'b0;
      datahigh_wren = 1'b0;
      outreg_wren   = 1'b0;
      REG_OUT       = inreg_out;
    end
    4'h2: begin
      cmd_wren      = REG_WREN;
      cmdaux_wren   = 1'b0;
      datalow_wren  = 1'b0;
      datahigh_wren = 1'b0;
      outreg_wren   = 1'b0;
      REG_OUT       = cmd_out;
    end
    4'h3: begin
      cmd_wren      = 1'b0;
      cmdaux_wren   = 1'b0;
      datalow_wren  = REG_WREN;
      datahigh_wren = 1'b0;
      outreg_wren   = 1'b0;
      REG_OUT       = datalow_out;
    end
    4'h4: begin
      cmd_wren      = 1'b0;
      cmdaux_wren   = 1'b0;
      datalow_wren  = 1'b0;
      datahigh_wren = REG_WREN;
      outreg_wren   = 1'b0;
      REG_OUT       = datahigh_out;
    end
    4'h5: begin
      cmd_wren      = 1'b0;
      cmdaux_wren   = 1'b0;
      datalow_wren  = 1'b0;
      datahigh_wren = 1'b0;
      outreg_wren   = REG_WREN;
      REG_OUT       = outreg_out;
    end
    4'h6: begin
      cmd_wren      = 1'b0;
      cmdaux_wren   = REG_WREN;
      datalow_wren  = 1'b0;
      datahigh_wren = 1'b0;
      outreg_wren   = 1'b0;
      REG_OUT       = cmdaux_out;
    end
    default: begin
      cmd_wren      = 1'b0;
      cmdaux_wren   = 1'b0;
      datalow_wren  = 1'b0;
      datahigh_wren = 1'b0;
      outreg_wren   = 1'b0;
      REG_OUT       = 16'hAAAA;
    end
  endcase
end

// Internal registers

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    cmd_out      <= {5'd5,11'd0};
    cmdaux_out   <= 16'd0;
    datalow_out  <= 16'd0;
    datahigh_out <= 16'd0;
    outreg_out   <= 16'd0;
  end else if (cmd_wren) 
    cmd_out      <= REG_IN;
  else if (datalow_wren) 
    datalow_out  <= REG_IN;
  else if (datahigh_wren) 
    datahigh_out <= REG_IN;
  else if (outreg_wren) 
    outreg_out   <= REG_IN;
  else if (cmdaux_wren)
    cmdaux_out   <= REG_IN;
end

  // General purpose IOs on ADC board
  // List of signals (DUT side <-> ADC-DAC board side): 
  // ADCMP603_Q_OUT(output) -> GPIO_OUT[14](input)
  // ADCMP603_Q_BAR_OUT(output) -> GPIO_OUT[13](input)
  // P82B96PWR_SCL_READ_OUT(output) -> GPIO_OUT[12](input)
  // P82B96PWR_SDA_READ_OUT(output) -> GPIO_OUT[11](input)
  // INTERRUPT_BAR_OUT(output) -> GPIO_OUT[10](input)
  // SCL_READ_OUT(output) -> GPIO_OUT[9](input)
  // SDA_READ_OUT(output) -> GPIO_OUT[8](input)
  // ADCMP603_LE_HYS_IN(input) <- GPIO_OUT[7](output)
  // P82B96PWR_SCL_WRITE_IN(input) <- GPIO_OUT[6](output)
  // P82B96PWR_SDA_WRITE_IN(input) <- GPIO_OUT[5](output)
  // SCL_WRITE_IN(input) <- GPIO_OUT[3](output)
  // SDA_WRITE_IN(input) <- GPIO_OUT[2](output)
  // DEV_ENABLE(input) <- GPIO_OUT[1](output)
  // other unassigned
  // <- GPIO_OUT[0](output)
  // <- GPIO_OUT[4](output)
  // -> GPIO_OUT[15](input)
  // <- GPCLK_OUT(output)

// Want to monitor ADCMP603_Q_BAR_OUT, ADCMP603_Q_OUT and read the differential IO error flags
assign inreg_out = {diffio_checker_errors,interrupt_transition,interrupt_init_state,4'd0,GPIO_IN[13],GPIO_IN[14]};
assign interrupt_bar = GPIO_IN[10];
// Want to set DEV_ENABLE, ADCMP603_LE_HYS_IN
assign GPIO_IN[7] = outreg_out[0];
assign GPIO_IN[1] = outreg_out[1];

assign data_in = {datahigh_out,datalow_out};
assign SDA_DIR_N = !sda_dir;
assign SCL_DIR_N = 1'b0;

//========================================================================
//                          Internal Modules 
//========================================================================

transactions_sm transactions_sm (

  // Clock interface
  .CLK(CLK),                            
  .RST_N(RST_N),                          
  
  // Start/CMD/busy interface
  .CMD({cmdaux_out,cmd_out}),                             
  .START(cmd_wren),                        
  .BUSY(BUSY),                        

  // Signals to external SPI/I2C interfaces (either on ADC-DAC board and DUT)
  .BUSY_ADC(busy_adc),
  .BUSY_DAC(busy_dac),
  .BUSY_WF(busy_wf),
  .BUSY_DIFFIO_CHECKER(busy_diffio_checker),
  .BUSY_INTERRUPT_CHECKER(busy_interrupt_checker),
  .BUSY_DP(busy_dp),
  .BUSY_I2C_DUT(busy_i2c_dut),
  .BUSY_I2C_DUT_AUX(busy_i2c_dut_aux),
  .BUSY_SPI_DUT(1'b0),
  .START_ADC(start_adc),
  .START_DAC(start_dac),
  .START_WF(start_wf),
  .START_DIFFIO_CHECKER(start_diffio_checker),
  .START_INTERRUPT_CHECKER(start_interrupt_checker),
  .START_DP(start_dp),
  .START_I2C_DUT(start_i2c_dut),
  .START_I2C_DUT_AUX(start_i2c_dut_aux),
  .START_SPI_DUT(),
  .OP_TYPE_SPI(op_type_spi),
   
  // Signals to the state machine that sends data to PC
  .BUSY_SEND(busy_send),
  .SEND_DATA(send_data),
  .SEND_HEADER(send_header),
  .SEND_TRAILER(send_trailer)
);

datatransm_sm datatransm_sm (

  // Clock interface
  .CLK(CLK),                                   // System clock (50MHz)
  .RST_N(RST_N),                               // Active low reset
  
  // Send/busy interface
  .SEND_DATA(send_data),
  .SEND_HEADER(send_header),
  .SEND_TRAILER(send_trailer),
  .BUSY(busy_send),                            // Indicates transaction in progress

  // Command
  .CMD({cmdaux_out,cmd_out}),                      

  // Data from SPI/I2C interfaces (either on ADC-DAC board and DUT)
  .DATA_ADC_BLOCK_0(data_adc_block_0),
  .DATA_ADC_BLOCK_1(data_adc_block_1),
  .DATA_ADC_BLOCK_2(data_adc_block_2),
  .DATA_ADC_BLOCK_3(data_adc_block_3),
  .DATA_DIFFIO_CHECKER(data_diffio_checker),
  .DATA_DP(24'd0),
  .DATA_I2C_DUT(data_i2c_dut),
  .DATA_I2C_DUT_AUX(data_i2c_dut_aux),
  .DATA_SPI_DUT(24'd0),

  // FIFO interface
  .FIFO_FULL(FIFO_FULL),
  .FIFO_DATA(FIFO_DATA_IN),
  .FIFO_WREN(FIFO_WREN)

);


// Generating a clock enable signal to reduce the frequency of the SPI interfaces
sclken_gen sclken_gen(
  .CLK(CLK),                 // System clock
  .RST_N(RST_N),             // Active low reset
  .SCLK(SCLK),               // SPI clock	
  .CLK_EN(sclk_en)           // Clock enable
);

// Generating all the SPI interfaces for the ADC board
// 0-31 - ADC interfaces
// 32-39 - DAC interfaces
// 40-47 - WF interfaces

genvar i;

generate
for (i = 0; i <= 47; i = i + 1) begin: spi_interface
spi_int spi_inter (
  .SCLK(CLK),
  .SCLK_EN(sclk_en),
  .START_SM(start_spi[i]),
  .OPT_TYPE(op_type_spi),
  .RST_N(RST_N),   
  .MISO(miso[i]),
  .NUM_BITS_TO_READ(num_bytes_to_rec*5'd8),
  .NUM_BITS_TO_WRITE(num_bytes_to_transm*5'd8),
  .DATA_TO_SLAVE(data_in[23:0]),                
  .SS_b(SS_b[i]),                         
  .DATA_FROM_SLAVE(data_out[(i+1)*24-1:i*24]),
  .BUSY(busy_spi[i]),
  .MOSI(mosi[i])  
);
end
endgenerate

// I2C interface (To communicate with the digital potentiometers on the ADC-DAC board)
i2c_master #(
  .INPUT_CLK_FREQUENCY(INPUT_CLK_FREQUENCY),   
  .BUS_CLK_FREQUENCY(I2C_CLK_FREQUENCY)       
)
i2c_master_adcboard(
  // Clock interface
  .CLK(CLK),                                 
  .RST_N(RST_N),                            
  
  // Start 
  .START(start_dp),                         
  .BUSY(busy_dp),                            

  // I2C data to be transmitted/received and number of bytes to be transmitted/received
  .I2C_DATA_TO_TRANSM({40'h0000000000,data_in[23:8]}),        
  .NUM_BYTES_TO_REC(5'd0),                                  
  .NUM_BYTES_TO_TRANSM({1'b0,num_bytes_to_transm}),        
  .REP_START(1'b0),                                           
  .SL_ADDR(data_in[6:0]),                                    
  .I2C_DATA_REC(),                                          
  
  // I2C signals to output slave device 
  .SDA_IN(SDA_IN),                           
  .SDA_OUT(SDA_OUT),                         
  .SDA_DIR(sda_dir),                        
  .SCL(SCL_OUT)                           
);

// I2C interface (To communicate with the I2C devices on the DUT board)
i2c_master #(
  .INPUT_CLK_FREQUENCY(INPUT_CLK_FREQUENCY), 
  .BUS_CLK_FREQUENCY(I2C_CLK_FREQUENCY)     
)
i2c_master_dutboard(
  // Clock interface
  .CLK(CLK),                                 
  .RST_N(RST_N),                             

  // Start 
  .START(start_i2c_dut),                      
  .BUSY(busy_i2c_dut),                        

  // I2C data to be transmitted/received and number of bytes to be transmitted/received
  .I2C_DATA_TO_TRANSM({40'h0000000000,data_in[23:8]}),
  .NUM_BYTES_TO_REC({2'd0,cmd_out[4:2]}),
  .NUM_BYTES_TO_TRANSM({1'b0,num_bytes_to_transm}),
  .REP_START(1'b0),
  .SL_ADDR(data_in[6:0]),
  .I2C_DATA_REC(data_out_i2c_dut),

  // I2C signals to output slave device (using GPIO)
  .SDA_IN(GPIO_IN[8]),                        
  .SDA_OUT(GPIO_IN[2]),                      
  .SDA_DIR(),                                
  .SCL(GPIO_IN[3])                         
);

// I2C interface (To communicate with the I2C devices on the DUT board using an auxiliary channel)
i2c_master #(
  .INPUT_CLK_FREQUENCY(INPUT_CLK_FREQUENCY),  
  .BUS_CLK_FREQUENCY(I2C_CLK_FREQUENCY)     
)
i2c_master_dutboard_aux(
  // Clock interface
  .CLK(CLK),                                
  .RST_N(RST_N),                            

  // Start 
  .START(start_i2c_dut_aux),                 
  .BUSY(busy_i2c_dut_aux),                   

  // I2C data to be transmitted/received and number of bytes to be transmitted/received
  .I2C_DATA_TO_TRANSM({40'h0000000000,data_in[23:8]}),
  .NUM_BYTES_TO_REC({2'd0,cmd_out[4:2]}),
  .NUM_BYTES_TO_TRANSM({1'b0,num_bytes_to_transm}),
  .REP_START(1'b0),
  .SL_ADDR(data_in[6:0]),
  .I2C_DATA_REC(data_out_i2c_dut_aux),

  // I2C signals to output slave device (using GPIO)
  .SDA_IN(GPIO_IN[11]),                      
  .SDA_OUT(GPIO_IN[5]),                      
  .SDA_DIR(),                                
  .SCL(GPIO_IN[6])                            
);

diffio_checker diffio_checker(
  // Clock interface
  .CLK(CLK),                                 
  .RST_N(RST_N),                        
  
  // Start/busy interface
  .START(start_diffio_checker),             
  .BUSY(busy_diffio_checker),              
  .FREQ_SCALE_FACTOR(cmd_out[15:11]),          

  // Buffer data/control signals
  .DIFF_IN(DIFF_IN[7:0]),
  .DIFF_OEN(DIFF_OEN[7:0]),
  .DIFF_OUT(DIFF_OUT[7:0]),
  
  // Error flags
  .ERROR_COUNTERS(data_diffio_checker),
  .ERROR_FLAGS(diffio_checker_errors)        
);

interrupt_checker_sm interrupt_checker_sm(
  // Clock interface
  .CLK(CLK),                       
  .RST_N(RST_N),                      

  // Start/busy interface from outside the Differential IO checker
  .START(start_interrupt_checker),
  .BUSY(busy_interrupt_checker),

  // Start/busy signals to/from other state machines
  .INTERRUPT_BAR(interrupt_bar),
  .INTERRUPT_INIT_STATE(interrupt_init_state),
  .INTERRUPT_TRANSITION(interrupt_transition)
);

//========================================================================
//                            Bus reassignments
//========================================================================

assign ADC_CS_IN[0] = |SS_b[7:0];
assign ADC_CS_IN[1] = |SS_b[15:8];
assign ADC_CS_IN[2] = |SS_b[23:16];
assign ADC_CS_IN[3] = |SS_b[31:24];
assign busy_adc = busy_spi[31:0];
assign busy_dac = busy_spi[39:32];
assign busy_wf = busy_spi[47:40];
assign start_spi[31:0] = start_adc;
assign start_spi[39:32] = start_dac;
assign start_spi[47:40] = start_wf;
assign miso[31:0] = ADC_DATA_OUT;
assign DAC_SYNC_IN = SS_b[39:32];
assign WF_FSYNC_IN = SS_b[47:40];
assign DAC_DATA_IN = mosi[39:32];
assign WF_DATA_IN = mosi[47:40];
assign data_i2c_dut = data_out_i2c_dut[23:0];   
assign data_i2c_dut_aux = data_out_i2c_dut_aux[23:0]; 
assign num_bytes_to_rec[1:0] = cmd_out[3:2];
assign num_bytes_to_transm[1:0] = cmd_out[1:0];

generate
for (i = 0; i <= 31; i = i + 1) begin: dataout_conn
  if (i < 8) 
    assign data_adc_block_0[(i+1)*12 - 1: i*12] = data_out[(2*i+1)*12 + 1:2*i*12 + 2];
  else if (i < 16)
    assign data_adc_block_1[(i+1-8)*12 - 1: (i-8)*12] = data_out[(2*i+1)*12 + 1:2*i*12 + 2];
  else if (i < 24)
    assign data_adc_block_2[(i+1-16)*12 - 1: (i-16)*12] = data_out[(2*i+1)*12 + 1:2*i*12 + 2];
  else if (i < 32)  
    assign data_adc_block_3[(i+1-24)*12 - 1: (i-24)*12] = data_out[(2*i+1)*12 + 1:2*i*12 + 2];
end
endgenerate

endmodule
