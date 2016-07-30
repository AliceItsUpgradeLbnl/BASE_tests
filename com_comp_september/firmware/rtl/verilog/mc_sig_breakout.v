//=============================================================================
// Title: Mezzanine card
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 3/16/2016
// Last modified: 3/16/2016 (AC)
// Functionality: Breaks down daughter card SE connectors into interfaces
//                for the ADC-DAC board 
//=============================================================================

module mc_sig_breakout (
  // Interface to single ended IO buffers
  input  [19:0] CON3_IN, CON4_IN, CON5_IN, CON6_IN, CON7_IN, CON8_IN, CON12_IN,
  output [19:0] CON3_OEN, CON4_OEN, CON5_OEN, CON6_OEN, CON7_OEN, CON8_OEN, CON12_OEN,
  output [19:0] CON3_OUT, CON4_OUT, CON5_OUT, CON6_OUT, CON7_OUT, CON8_OUT, CON12_OUT,  
  
  // Interface to differential IO buffers
  input  [9:0] CON17_DIFF_IN, CON18_DIFF_IN,
  output [9:0] CON17_DIFF_OEN, CON18_DIFF_OEN,  
  output [9:0] CON17_DIFF_OUT, CON18_DIFF_OUT,  
  
  // Interfaces to FPGA core
  
  // I2C interface to communicate with the digital potentiometers on the ADC-DAC board
  input  SCL_DIR_N, // 0 == output enabled -> Master (FPGA) to Slave direction
  input  SCL_OUT,   // From master (FPGA) to slave
  input  SDA_DIR_N, // 0 == output enabled -> Master (FPGA) to Slave direction
  input  SDA_OUT,   // From master (FPGA) to slave
  output SCL_IN,    // From slave to master (FPGA)
  output SDA_IN,    // From slave to master (FPGA)

  // ADC interface
  input  [1:0] ADC_CLK_IN, 
  input  [3:0] ADC_CS_IN,
  output [31:0] ADC_DATA_OUT,
  
  // DAC interface
  input  [1:0] DAC_CLK_IN,
  input  [7:0] DAC_DATA_IN,
  input  [7:0] DAC_SYNC_IN,
  
  // Waveform generator interface
  input  WF_CLK,  
  input  [7:0] WF_DATA_IN,
  input  [7:0] WF_FSYNC_IN,

  // General purpose IOs on ADC board
  input  GPCLK_IN,         // General purpose clock
  inout  [15:0] GPIO_IN,   // General purpose data lines
  
  // Differential IOs
  input  [19:0] DIFF_OEN,  // Output enable of the FPGA
  input  [19:0] DIFF_OUT,  // Outputs of the FPGA
  output [19:0] DIFF_IN    // Inputs of the FPGA
);

//========================================================================
//                          Internal Signals 
//========================================================================

wire [1:0] GPIO_DIR_N;

//========================================================================
//                          Conn generation 
//========================================================================

// Using only the 6 FCS8 connectors all on one of the large sides of the daughter card
// Unused connectors must have related buffers disabled (input)
assign CON12_OEN = 20'hFFFFF;

// Setting direction for the used connectors (unused pins will be set as inputs)
// Special case: Connector 3. CON3_OEN signals for FPGA IO buffers and GPIO_DIR_N for buffers on the ADC-DAC board must not conflict (shorts otherwise!) 
assign CON3_OEN   = 20'h003FD;   // (1 msb output, 1 output, 8 outputs, 8 inputs, 1 output, lsb unused)
assign GPIO_DIR_N = {1'b1,1'b0}; // Control lines for GPIO data lines (GPIO_DIR_N[0] controls GPIO_IN[7:0], GPIO_DIR_N[1] controls GPIO_IN[15:8]), 0 = From FPGA to DUT board
// Other cases
assign CON4_OEN   = 20'h007FF;   // (9 msb outputs, 11 lsb connected to header - unused for now)
assign CON5_OEN   = 20'h1FFFF;   // (3 msb outputs, 16 bits inputs, lsb unused)
assign CON6_OEN   = 20'hFFFF1;   // (16 msb inputs, 3 bits outputs, lsb unused)
assign CON7_OEN   = 20'h007FF;   // (9 msb outputs, 11 lsb connected to header - unused for now)
assign CON8_OEN   = {17'b00000000000000000,SDA_OUT,SCL_OUT,1'b1}; // (17 msb outputs, 2 bits bidir, lsb unused) Notice that when SDA_OUT and SCL_OUT are high, lines are z

// assigning connector IO signals 

//////////////////////////// Connector 3 //////////////////////////////////

// Inputs

assign GPIO_IN[15]  = CON3_IN[2];
assign GPIO_IN[14]  = CON3_IN[3];
assign GPIO_IN[13]  = CON3_IN[4];
assign GPIO_IN[12]  = CON3_IN[5];
assign GPIO_IN[11]  = CON3_IN[6];
assign GPIO_IN[10]  = CON3_IN[7];
assign GPIO_IN[9]   = CON3_IN[8];
assign GPIO_IN[8]   = CON3_IN[9];

// Outputs

assign CON3_OUT[19]   = GPCLK_IN;
assign CON3_OUT[18]   = GPIO_DIR_N[0];
assign CON3_OUT[17]   = GPIO_IN[0];
assign CON3_OUT[16]   = GPIO_IN[1];
assign CON3_OUT[15]   = GPIO_IN[2];
assign CON3_OUT[14]   = GPIO_IN[3];
assign CON3_OUT[13]   = GPIO_IN[4];
assign CON3_OUT[12]   = GPIO_IN[5];
assign CON3_OUT[11]   = GPIO_IN[6];
assign CON3_OUT[10]   = GPIO_IN[7];
assign CON3_OUT[9]    = GPIO_IN[8];
assign CON3_OUT[8]    = GPIO_IN[9];
assign CON3_OUT[7]    = GPIO_IN[10];
assign CON3_OUT[6]    = GPIO_IN[11];
assign CON3_OUT[5]    = GPIO_IN[12];
assign CON3_OUT[4]    = GPIO_IN[13];
assign CON3_OUT[3]    = GPIO_IN[14];
assign CON3_OUT[2]    = GPIO_IN[15];
assign CON3_OUT[1]    = GPIO_DIR_N[1];
assign CON3_OUT[0]    = 1'b0;  // Unused

//////////////////////////// Connector 4 //////////////////////////////////

// Inputs (no inputs used)

// Outputs

assign CON4_OUT[19]   = DAC_DATA_IN[0];
assign CON4_OUT[18]   = DAC_SYNC_IN[0];
assign CON4_OUT[17]   = DAC_DATA_IN[1];
assign CON4_OUT[16]   = DAC_SYNC_IN[1];
assign CON4_OUT[15]   = DAC_DATA_IN[2];
assign CON4_OUT[14]   = DAC_SYNC_IN[2];
assign CON4_OUT[13]   = DAC_DATA_IN[3];
assign CON4_OUT[12]   = DAC_SYNC_IN[3];
assign CON4_OUT[11]   = DAC_CLK_IN[0];
assign CON4_OUT[10:0] = 11'd0;  // Unused (header)

//////////////////////////// Connector 5 //////////////////////////////////

// Inputs

assign ADC_DATA_OUT[0]  = CON5_IN[16];
assign ADC_DATA_OUT[1]  = CON5_IN[15];
assign ADC_DATA_OUT[2]  = CON5_IN[14];
assign ADC_DATA_OUT[3]  = CON5_IN[13];
assign ADC_DATA_OUT[4]  = CON5_IN[12];
assign ADC_DATA_OUT[5]  = CON5_IN[11];
assign ADC_DATA_OUT[6]  = CON5_IN[10];
assign ADC_DATA_OUT[7]  = CON5_IN[9];
assign ADC_DATA_OUT[8]  = CON5_IN[8];
assign ADC_DATA_OUT[9]  = CON5_IN[7];
assign ADC_DATA_OUT[10] = CON5_IN[6];
assign ADC_DATA_OUT[11] = CON5_IN[5];
assign ADC_DATA_OUT[12] = CON5_IN[4];
assign ADC_DATA_OUT[13] = CON5_IN[3];
assign ADC_DATA_OUT[14] = CON5_IN[2];
assign ADC_DATA_OUT[15] = CON5_IN[1];

// Outputs

assign CON5_OUT[19]   = ADC_CLK_IN[0];
assign CON5_OUT[18]   = ADC_CS_IN[0];
assign CON5_OUT[17]   = ADC_CS_IN[1];
assign CON5_OUT[16:1] = 16'h0000; // Config as inputs
assign CON5_OUT[0]    = 1'b0; // Unused

//////////////////////////// Connector 6 //////////////////////////////////

// Inputs

assign ADC_DATA_OUT[16] = CON6_IN[19];
assign ADC_DATA_OUT[17] = CON6_IN[18];
assign ADC_DATA_OUT[18] = CON6_IN[17];
assign ADC_DATA_OUT[19] = CON6_IN[16];
assign ADC_DATA_OUT[20] = CON6_IN[15];
assign ADC_DATA_OUT[21] = CON6_IN[14];
assign ADC_DATA_OUT[22] = CON6_IN[13];
assign ADC_DATA_OUT[23] = CON6_IN[12];
assign ADC_DATA_OUT[24] = CON6_IN[11];
assign ADC_DATA_OUT[25] = CON6_IN[10];
assign ADC_DATA_OUT[26] = CON6_IN[9];
assign ADC_DATA_OUT[27] = CON6_IN[8];
assign ADC_DATA_OUT[28] = CON6_IN[7];
assign ADC_DATA_OUT[29] = CON6_IN[6];
assign ADC_DATA_OUT[30] = CON6_IN[5];
assign ADC_DATA_OUT[31] = CON6_IN[4];

// Outputs

assign CON6_OUT[19:4] = 16'h0000; // Config as inputs
assign CON6_OUT[3]    = ADC_CS_IN[2];
assign CON6_OUT[2]    = ADC_CS_IN[3];
assign CON6_OUT[1]    = ADC_CLK_IN[1]; // Config as inputs
assign CON6_OUT[0]    = 1'b0; // Unused

//////////////////////////// Connector 7 //////////////////////////////////

// Inputs (no inputs used)

// Outputs

assign CON7_OUT[19]   = DAC_DATA_IN[4];
assign CON7_OUT[18]   = DAC_SYNC_IN[4];
assign CON7_OUT[17]   = DAC_DATA_IN[5];
assign CON7_OUT[16]   = DAC_SYNC_IN[5];
assign CON7_OUT[15]   = DAC_DATA_IN[6];
assign CON7_OUT[14]   = DAC_SYNC_IN[6];
assign CON7_OUT[13]   = DAC_DATA_IN[7];
assign CON7_OUT[12]   = DAC_SYNC_IN[7];
assign CON7_OUT[11]   = DAC_CLK_IN[1];
assign CON7_OUT[10:0] = 11'd0; // Unused (header)

//////////////////////////// Connector 8 //////////////////////////////////

// Inputs

assign SDA_IN         = CON8_IN[2];
assign SCL_IN         = CON8_IN[1];

// Outputs

assign CON8_OUT[19]   = WF_DATA_IN[0];
assign CON8_OUT[18]   = WF_FSYNC_IN[0];
assign CON8_OUT[17]   = WF_DATA_IN[1];
assign CON8_OUT[16]   = WF_FSYNC_IN[1];
assign CON8_OUT[15]   = WF_DATA_IN[2];
assign CON8_OUT[14]   = WF_FSYNC_IN[2];
assign CON8_OUT[13]   = WF_DATA_IN[3];
assign CON8_OUT[12]   = WF_FSYNC_IN[3];
assign CON8_OUT[11]   = WF_CLK;
assign CON8_OUT[10]   = WF_DATA_IN[4];
assign CON8_OUT[9]    = WF_FSYNC_IN[4];
assign CON8_OUT[8]    = WF_DATA_IN[5];
assign CON8_OUT[7]    = WF_FSYNC_IN[5];
assign CON8_OUT[6]    = WF_DATA_IN[6];
assign CON8_OUT[5]    = WF_FSYNC_IN[6];
assign CON8_OUT[4]    = WF_DATA_IN[7];
assign CON8_OUT[3]    = WF_FSYNC_IN[7];
assign CON8_OUT[2]    = SDA_OUT;
assign CON8_OUT[1]    = SCL_OUT;
assign CON8_OUT[0]    = 1'b0;

//////////////////////////// Connector 17 /////////////////////////////////

// Inputs

assign DIFF_IN[0] = CON17_DIFF_IN[0];
assign DIFF_IN[1] = CON17_DIFF_IN[1];
assign DIFF_IN[2] = CON17_DIFF_IN[2];
assign DIFF_IN[3] = CON17_DIFF_IN[3];
assign DIFF_IN[4] = CON17_DIFF_IN[4];
assign DIFF_IN[5] = CON17_DIFF_IN[5];
assign DIFF_IN[6] = CON17_DIFF_IN[6];
assign DIFF_IN[7] = CON17_DIFF_IN[7];
assign DIFF_IN[8] = CON17_DIFF_IN[8];
assign DIFF_IN[9] = CON17_DIFF_IN[9];

// Outputs

assign CON17_DIFF_OUT[0]   = DIFF_OUT[0];
assign CON17_DIFF_OUT[1]   = DIFF_OUT[1];
assign CON17_DIFF_OUT[2]   = DIFF_OUT[2];
assign CON17_DIFF_OUT[3]   = DIFF_OUT[3];
assign CON17_DIFF_OUT[4]   = DIFF_OUT[4];
assign CON17_DIFF_OUT[5]   = DIFF_OUT[5];
assign CON17_DIFF_OUT[6]   = DIFF_OUT[6];
assign CON17_DIFF_OUT[7]   = DIFF_OUT[7];
assign CON17_DIFF_OUT[8]   = DIFF_OUT[8];
assign CON17_DIFF_OUT[9]   = DIFF_OUT[9];

assign CON17_DIFF_OEN[0]   = DIFF_OEN[0];
assign CON17_DIFF_OEN[1]   = DIFF_OEN[1];
assign CON17_DIFF_OEN[2]   = DIFF_OEN[2];
assign CON17_DIFF_OEN[3]   = DIFF_OEN[3];
assign CON17_DIFF_OEN[4]   = DIFF_OEN[4];
assign CON17_DIFF_OEN[5]   = DIFF_OEN[5];
assign CON17_DIFF_OEN[6]   = DIFF_OEN[6];
assign CON17_DIFF_OEN[7]   = DIFF_OEN[7];
assign CON17_DIFF_OEN[8]   = DIFF_OEN[8];
assign CON17_DIFF_OEN[9]   = DIFF_OEN[9];


//////////////////////////// Connector 18 /////////////////////////////////

// Inputs

assign DIFF_IN[10] = CON18_DIFF_IN[0];
assign DIFF_IN[11] = CON18_DIFF_IN[1];
assign DIFF_IN[12] = CON18_DIFF_IN[2];
assign DIFF_IN[13] = CON18_DIFF_IN[3];
assign DIFF_IN[14] = CON18_DIFF_IN[4];
assign DIFF_IN[15] = CON18_DIFF_IN[5];
assign DIFF_IN[16] = CON18_DIFF_IN[6];
assign DIFF_IN[17] = CON18_DIFF_IN[7];
assign DIFF_IN[18] = CON18_DIFF_IN[8];
assign DIFF_IN[19] = CON18_DIFF_IN[9];

// Outputs

assign CON18_DIFF_OUT[0]   = DIFF_OUT[10];
assign CON18_DIFF_OUT[1]   = DIFF_OUT[11];
assign CON18_DIFF_OUT[2]   = DIFF_OUT[12];
assign CON18_DIFF_OUT[3]   = DIFF_OUT[13];
assign CON18_DIFF_OUT[4]   = DIFF_OUT[14];
assign CON18_DIFF_OUT[5]   = DIFF_OUT[15];
assign CON18_DIFF_OUT[6]   = DIFF_OUT[16];
assign CON18_DIFF_OUT[7]   = DIFF_OUT[17];
assign CON18_DIFF_OUT[8]   = DIFF_OUT[18];
assign CON18_DIFF_OUT[9]   = DIFF_OUT[19];

assign CON18_DIFF_OEN[0]   = DIFF_OEN[10];
assign CON18_DIFF_OEN[1]   = DIFF_OEN[11];
assign CON18_DIFF_OEN[2]   = DIFF_OEN[12];
assign CON18_DIFF_OEN[3]   = DIFF_OEN[13];
assign CON18_DIFF_OEN[4]   = DIFF_OEN[14];
assign CON18_DIFF_OEN[5]   = DIFF_OEN[15];
assign CON18_DIFF_OEN[6]   = DIFF_OEN[16];
assign CON18_DIFF_OEN[7]   = DIFF_OEN[17];
assign CON18_DIFF_OEN[8]   = DIFF_OEN[18];
assign CON18_DIFF_OEN[9]   = DIFF_OEN[19];

endmodule
