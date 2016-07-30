//=============================================================================
// Title: Generic I2C interface
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 10/16/2015
// Last modified: 10/16/2015 (AC)
// Description: this can initiate customized I2C transactions (max 7 bytes
//              write + max 31 byte read). At now no ACK check functionalities
//              and no clock stretching is implemented. I2C SCL has a duty ratio
//              of 33%.
//=============================================================================

module i2c_master #(
  parameter INPUT_CLK_FREQUENCY = 50000000,   // Input clock frequency (req: INPUT_CLK_FREQUENCY/BUS_CLK_FREQUENCY >= 20)
  parameter BUS_CLK_FREQUENCY = 100000        // I2C SCL frequency (fixed duty ratio = 33%) 
)
(
  // Clock interface
  input  CLK,                                 // System clock (50MHz)
  input  RST_N,                               // Active low reset
  
  // Signals from external control interface
  input  REP_START,                           // Repeate start after write phase
  input  START,                               // Start an I2C transaction
  output BUSY,                                // Indicates transaction in progress

  // I2C data to transmit/receive and number of bytes to transmit/receive
  input  [55:0]  I2C_DATA_TO_TRANSM,          // I2C data to transmit
  input  [4:0]   NUM_BYTES_TO_REC,            // Number of bytes to be received from slave device
  input  [2:0]   NUM_BYTES_TO_TRANSM,         // Number of bytes to be transmitted to slave device (excluded the first byte that contains the slave address)
  input  [6:0]   SL_ADDR,                     // I2C slave address
  output [247:0] I2C_DATA_REC,                // 31 bytes max of data from a slave device
  
  // I2C signals to output slave device 
  input  SDA_IN,                              // Serial input data from SDA line
  output SDA_OUT,                             // Serial output data to SDA line
  output SDA_DIR,                             // Direction of SDA line
  output SCL                                  // Serial clock output to SCL line
);

//========================================================================
//                           Internal Signals 
//========================================================================

wire clk_en;
wire busy_main_sm;
wire [63:0] data_from_core;
wire latch_from_core;
wire latch_to_core;
wire load_output_shiftreg;
wire [4:0] num_bytes_to_rec_out;
wire [2:0] num_bytes_to_transm_out;
wire read_type;
wire rep_start_int;
wire rst_core_side_register;
wire rw;
wire scl_sm;
wire sda_dir_sm;
wire sda_out_sm;
wire sda_sel_sm;
wire shift_in;
wire shift_out;
wire start_i2c;
wire start_main_sm;

reg  start_shiftreg;

//========================================================================
//                         Combinatorial logic
//========================================================================

assign start_i2c = (START && !start_shiftreg) && !BUSY;

//========================================================================
//                           Sequential logic
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    start_shiftreg   <= 1'b0;
  else begin
    start_shiftreg   <= START;
  end
end

//========================================================================
//                        I2C Start State Machine 
//========================================================================

i2c_start_sm i2c_start_sm(
  .BUSY_MAIN_SM(busy_main_sm),          
  .CLK(CLK),                               
  .RST_N(RST_N),                            
  .START_I2C(start_i2c),                  
  .BUSY(BUSY),                              
  .START_MAIN_SM(start_main_sm)             
);

//========================================================================
//                          I2C Input Registers
//========================================================================

i2c_regs i2c_regs(
  // Clock interface
  .CLK(CLK),                                          
  .RST_N(RST_N),                                       
  
  // Input data interface          
  .I2C_DATA_TO_TRANSM(I2C_DATA_TO_TRANSM),      
  .LATCH_INPUT_DATA(start_i2c),                          
  .NUM_BYTES_TO_REC_IN(NUM_BYTES_TO_REC),           
  .NUM_BYTES_TO_TRANSM_IN(NUM_BYTES_TO_TRANSM),      
  .REP_START_IN(REP_START),                               
  .SL_ADDR(SL_ADDR),                                     
  .DATA_FROM_CORE(data_from_core),                       
  .NUM_BYTES_TO_REC_OUT(num_bytes_to_rec_out),        
  .NUM_BYTES_TO_TRANSM_OUT(num_bytes_to_transm_out), 
  .REP_START_OUT(rep_start_int),                         
  .RW(rw)                                                
);

//========================================================================
//                          I2C clock divider
//========================================================================

i2c_clock_divider #(
  .INPUT_CLK_FREQUENCY(INPUT_CLK_FREQUENCY),       
  .BUS_CLK_FREQUENCY(BUS_CLK_FREQUENCY)            
)
i2c_clock_divider (
  .CLK(CLK),                                   
  .RST_N(RST_N),                   
  .CLK_EN(clk_en)                              
);

//========================================================================
//                        I2C Main state machine
//========================================================================

i2c_main_sm i2c_main_sm (
  // Clock interface
  .CLK(CLK),                                     
  .CLK_EN(clk_en),                       
  .RST_N(RST_N),                              

  // Main SM interface
  .START_SM(start_main_sm),                         
  .REP_START(rep_start_int),                       
  .RW(rw),
  .NUM_BYTES_TO_REC(num_bytes_to_rec_out),      
  .NUM_BYTES_TO_TRANSM(num_bytes_to_transm_out),
  .BUSY(busy_main_sm),                               

  // Interface to serializer
  .LATCH_FROM_CORE(latch_from_core),                
  .LOAD_OUTPUT_SHIFTREG(load_output_shiftreg),   
  .SHIFT_OUT(shift_out),                          
  
  // Interface to deserializer
  .LATCH_TO_CORE(latch_to_core),                     
  .RST_CORE_SIDE_REG(rst_core_side_register),       
  .SHIFT_IN(shift_in),                             
  
  // I2C output signals (to be complemented with data and filtered)
  .SDA_SEL_SM(sda_sel_sm),                         
  .SCL_SM(scl_sm),                                  
  .SDA_DIR_SM(sda_dir_sm),                        
  .SDA_OUT_SM(sda_out_sm)                           
);

//========================================================================
//                          I2C deserializer
//========================================================================

i2c_deserializer i2c_deserializer (
  // Clock interface
  .CLK(CLK),                                   
  .CLK_EN(clk_en),                            
  .RST_N(RST_N),                              
  
  // Control interface
  .LATCH_CORE_SIDE_REG(latch_to_core),          
  .RST_CORE_SIDE_REG(rst_core_side_register),  
  .SHIFT_IN(shift_in),                        

  // I2C input signals from slave device 
  .SDA_IN(SDA_IN),                            

  // Data
  .I2C_DATA_REC(I2C_DATA_REC)                 
);

//========================================================================
//                           I2C serializer
//========================================================================

i2c_serializer i2c_serializer(
  // Clock interface
  .CLK(CLK),                                   
  .CLK_EN(clk_en),                             
  .RST_N(RST_N),                               

  // Shiftreg interface
  .LOAD_SHIFTREG(load_output_shiftreg),          
  .SHIFT_OUT(shift_out),                        

  // Data to be transmitted to slave device
  .DATA_FROM_CORE(data_from_core),             
  .LATCH_CORE_SIDE_REG(latch_from_core),       
  
  // I2C input signals from state machine (to be complemented with data and filtered)
  .SDA_SEL_SM(sda_sel_sm),                    
  .SCL_SM(scl_sm),                             
  .SDA_DIR_SM(sda_dir_sm),                    
  .SDA_OUT_SM(sda_out_sm),                      

  // I2C output signals to slave device 
  .SCL(SCL),                                 
  .SDA_DIR(SDA_DIR),                           
  .SDA_OUT(SDA_OUT)                            
);

endmodule
