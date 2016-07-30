//=============================================================================
// Title: Radiation tests interface
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 3/16/2015
// Last modified: 3/16/2015 (AC)
// Description: contains interfaces for communicating with SPI/I2C devices on the
//              radiation testing boards
//=============================================================================

module rad_test_int #(
  parameter INPUT_CLK_FREQUENCY = 50000000,   // Input clock frequency
  parameter I2C_CLK_FREQUENCY = 50000,        // I2C clock frequency
  parameter SPI_CLK_FREQUENCY = 25000000      // SPI clock frequency
)
(

  ///////////////////////////////// IO signals -- Control interface side /////////////////////////////////////////////////

  // Clock interface
  input  CLK,                                 // System clock (50MHz)
  input  RST_N,                               // Active low reset
  
  output BUSY,                                // Indicates transaction in progress

  // Reg interface
  input  [3:0] REG_ADDR,                      // Reg address   
  input  [15:0] REG_IN,                       // Reg input data bus
  input  REG_WREN,                            // Reg write enable 
  output [15:0] REG_OUT,                      // Reg output data bus

  // FIFO interface
  input  FIFO_FULL,                                
  output [31:0] FIFO_DATA_IN,
  output FIFO_WREN,               

  // SPI CLK
  input  SPI_CLK,
  
  ///////////////////////////////// IO signals -- Daughter card side /////////////////////////////////////////////////////

  inout [100:1] DC_A,
  inout [20:1]  DC_A_N,
  inout [20:1]  DC_A_P,
  inout [100:1] DC_B

);

//========================================================================
//                          Internal Signals 
//========================================================================

wire [3:0] adc_cs_in;
wire [31:0] adc_data_out;
wire [7:0] dac_data_in;
wire [7:0] dac_sync_in;
wire [15:0] gpio_in;
wire [7:0] wf_data_in;
wire [7:0] wf_fsync_in;

wire scl_dir_n;
wire scl_in;
wire scl_out;
wire sda_dir_n;
wire sda_in;
wire sda_out;

wire [19:0] diff_in;
wire [19:0] diff_oen;
wire [19:0] diff_out;

//========================================================================
//                          Internal Modules 
//========================================================================

mc_int mc_int(
  // FPGA Ios
  .DC_A(DC_A),
  .DC_A_N(DC_A_N),
  .DC_A_P(DC_A_P),
  .DC_B(DC_B),

  // Interfaces to FPGA core
  
  // I2C interface to communicate with the digital potentiometers on the ADC-DAC board
  .SCL_DIR_N(scl_dir_n),   // 0 == output enabled -> Master (FPGA) to Slave direction
  .SCL_OUT(scl_out),       // From master (FPGA) to slave
  .SDA_DIR_N(sda_dir_n),   // 0 == output enabled -> Master (FPGA) to Slave direction
  .SDA_OUT(sda_out),       // From master (FPGA) to slave
  .SCL_IN(scl_in),         // From slave to master (FPGA)
  .SDA_IN(sda_in),         // From slave to master (FPGA)

  // ADC interface
  .ADC_CLK_IN({SPI_CLK,SPI_CLK}), 
  .ADC_CS_IN(adc_cs_in),
  .ADC_DATA_OUT(adc_data_out),
  
  // DAC interface
  .DAC_CLK_IN({SPI_CLK,SPI_CLK}),
  .DAC_DATA_IN(dac_data_in),
  .DAC_SYNC_IN(dac_sync_in),
  
  // Waveform generator interface
  .WF_CLK(SPI_CLK),  
  .WF_DATA_IN(wf_data_in),
  .WF_FSYNC_IN(wf_fsync_in),

  // General purpose single ended IOs on ADC board
  .GPCLK_IN(1'b0),          // General purpose clock (unused in this application)
  .GPIO_IN(gpio_in),        // General purpose data lines

  // General purpose differential IOs on ADC board
  .DIFF_OEN(diff_oen),      // Control enable signals of the IO buffers of the FPGA (active low)
  .DIFF_OUT(diff_out),      // Differential outputs of the FPGA
  .DIFF_IN(diff_in)         // Differential inputs of the FPGA

);

spi_i2c_block #(
  .INPUT_CLK_FREQUENCY(INPUT_CLK_FREQUENCY),
  .I2C_CLK_FREQUENCY(I2C_CLK_FREQUENCY),
  .SPI_CLK_FREQUENCY(SPI_CLK_FREQUENCY)
)
spi_i2c_block (
  // Clock interface
  .CLK(CLK),                                
  .RST_N(RST_N),                              
                          
  .BUSY(BUSY),

  // Reg interface
  .REG_ADDR(REG_ADDR),                   // Reg address   
  .REG_IN(REG_IN),                       // Reg input data bus
  .REG_WREN(REG_WREN),                   // Reg write enable 
  .REG_OUT(REG_OUT),                     // Reg output data bus        

  .SCLK(SPI_CLK),

  // FIFO interface
  .FIFO_FULL(FIFO_FULL),     
  .FIFO_DATA_IN(FIFO_DATA_IN),
  .FIFO_WREN(FIFO_WREN),

  // Signals to/from SPI/I2C slaves
  
  // I2C interface to communicate with the digital potentiometers on the ADC-DAC board
  .SCL_DIR_N(scl_dir_n),   // 0 == output enabled -> Master (FPGA) to Slave direction
  .SCL_OUT(scl_out),       // From master (FPGA) to slave
  .SDA_DIR_N(sda_dir_n),   // 0 == output enabled -> Master (FPGA) to Slave direction
  .SDA_OUT(sda_out),       // From master (FPGA) to slave
  .SCL_IN(scl_in),         // From slave to master (FPGA)
  .SDA_IN(sda_in),         // From slave to master (FPGA)

  // ADC interface
  .ADC_CS_IN(adc_cs_in),
  .ADC_DATA_OUT(adc_data_out),
  
  // DAC interface
  .DAC_DATA_IN(dac_data_in),
  .DAC_SYNC_IN(dac_sync_in),
  
  // Waveform generator interface 
  .WF_DATA_IN(wf_data_in),
  .WF_FSYNC_IN(wf_fsync_in),

  // General purpose IOs on ADC board
  .GPIO_IN(gpio_in),
  
  // Differential signals on ADC board
  .DIFF_IN(diff_in),             
  .DIFF_OEN(diff_oen),            
  .DIFF_OUT(diff_out)            

);

endmodule
