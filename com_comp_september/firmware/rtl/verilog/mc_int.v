//=============================================================================
// Title: Mezzanine card interface top
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 3/16/2016
// Last modified: 7/28/2016 (AC)
// Functionality: Breaks down the daughter card IO buses into interfaces
//                for the ADC-DAC board
//=============================================================================

module mc_int (
  // FPGA IOs
  inout  [100:1] DC_A,
  inout  [20:1]  DC_A_N,
  inout  [20:1]  DC_A_P,
  inout  [100:1] DC_B,

  // Interfaces to FPGA core
  
  // I2C interface to communicate with the digital potentiometers on the ADC-DAC board
  input  SCL_DIR_N, // 0 == output enabled -> Master (FPGA) to Slave direction
  input  SCL_OUT,   // From master (FPGA) to slave
  input  SDA_DIR_N, // 0 == output enabled -> Master (FPGA) to Slave direction
  input  SDA_OUT,   // From master (FPGA) to slave
  output SCL_IN,    // From slave to master (FPGA)
  output SDA_IN,    // From slave to master (FPGA)

  // ADC interface
  input  [1:0]  ADC_CLK_IN, 
  input  [3:0]  ADC_CS_IN,
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
  input  [19:0] DIFF_OEN,  // Output enable signals of the FPGA
  input  [19:0] DIFF_OUT,  // Outputs of the FPGA
  output [19:0] DIFF_IN    // Inputs of the FPGA
  
);
		
//========================================================================
//                          Internal Signals 
//========================================================================

wire [19:0] CON3_IN, CON4_IN, CON5_IN, CON6_IN, CON7_IN, CON8_IN, CON12_IN;
wire [19:0] CON3_OEN, CON4_OEN, CON5_OEN, CON6_OEN, CON7_OEN, CON8_OEN, CON12_OEN;
wire [19:0] CON3_OUT, CON4_OUT, CON5_OUT, CON6_OUT, CON7_OUT, CON8_OUT, CON12_OUT;
wire [9:0]  CON17_DIFF_IN, CON18_DIFF_IN;
wire [9:0]  CON17_DIFF_OEN, CON18_DIFF_OEN;
wire [9:0]  CON17_DIFF_OUT, CON18_DIFF_OUT;

//========================================================================
//                          Conn generation 
//========================================================================

genvar i;

// Differential signals
generate
for (i = 1; i <= 20; i = i + 1) begin: DC_A_DIFF_buffers
  if (i <= 10) begin // U17
    IOBUFDS DC_A_DIFF_BUF (
      .O(CON17_DIFF_IN[i - 1]),   // Buffer output
      .IO(DC_A_P[i]),             // Diff_p inout (connect directly to top-level port)
      .IOB(DC_A_N[i]),            // Diff_n inout (connect directly to top-level port)
      .I(CON17_DIFF_OUT[i - 1]),  // Buffer input
      .T(CON17_DIFF_OEN[i - 1])   // 3-state enable input  
    );
  end else begin // U18
    IOBUFDS DC_A_DIFF_BUF (
      .O(CON18_DIFF_IN[i - 11]),  // Buffer output
      .IO(DC_A_P[i]),             // Diff_p inout (connect directly to top-level port)
      .IOB(DC_A_N[i]),            // Diff_n inout (connect directly to top-level port)
      .I(CON18_DIFF_OUT[i - 11]), // Buffer input
      .T(CON18_DIFF_OEN[i - 11])  // 3-state enable input  
    );  
  end
end
endgenerate

// Single ended signals on daughter card connector A
generate
for (i = 1; i <= 100; i = i + 1) begin: DC_A_buffers
    if (i <= 60 && i > 40) begin // U3
      IOBUF DC_A_BUF (
        .O(CON3_IN[i-41]),
        .IO(DC_A[i]),
        .I(CON3_OUT[i-41]),
        .T(CON3_OEN[i-41])
      );
    end else if (i <= 76 && i > 60) begin // U5
      IOBUF DC_A_BUF (
        .O(CON5_IN[19-(i-58)]),
        .IO(DC_A[i]),
        .I(CON5_OUT[19-(i-58)]),
        .T(CON5_OEN[19-(i-58)])
      );
    end else if (i <= 97 && i > 80) begin // U4
      IOBUF DC_A_BUF (
        .O(CON4_IN[19-(i-81)]),
        .IO(DC_A[i]),
        .I(CON4_OUT[19-(i-81)]),
        .T(CON4_OEN[19-(i-81)])
      );
    end else if (i <= 100 && i > 97) begin // U5
      IOBUF DC_A_BUF (
        .O(CON5_IN[19-(i-98)]),
        .IO(DC_A[i]),
        .I(CON5_OUT[19-(i-98)]),
        .T(CON5_OEN[19-(i-98)])
      );
    end
end
endgenerate

// Single ended signals on daughter card connector B
generate
for (i = 1; i <= 80; i = i + 1) begin: DC_B_buffers
  if (i <= 17 && i > 0) begin // U7
      IOBUF DC_B_BUF (
        .O(CON7_IN[i+2]),
        .IO(DC_B[i]),
        .I(CON7_OUT[i+2]),
        .T(CON7_OEN[i+2])
      );
  end else if (i <= 20 && i > 17) begin // U6
      IOBUF DC_B_BUF (
        .O(CON6_IN[i-17]),
        .IO(DC_B[i]),
        .I(CON6_OUT[i-17]),
        .T(CON6_OEN[i-17])
      );
  end else if (i <= 56 && i > 40) begin // U6
      IOBUF DC_B_BUF (
        .O(CON6_IN[i-37]),
        .IO(DC_B[i]),
        .I(CON6_OUT[i-37]),
        .T(CON6_OEN[i-37])
      );
  end else if (i <= 22 && i > 20) begin // U8
      IOBUF DC_B_BUF (
        .O(CON8_IN[i-20]),
        .IO(DC_B[i]),
        .I(CON8_OUT[i-20]),
        .T(CON8_OEN[i-20])
      ); 
  end else if (i <= 40 && i > 23) begin // U8
      IOBUF DC_B_BUF (
        .O(CON8_IN[i-21]),
        .IO(DC_B[i]),
        .I(CON8_OUT[i-21]),
        .T(CON8_OEN[i-21])
      ); 
    end else if (i <= 100 && i > 80) begin // U12
      IOBUF DC_B_BUF (
        .O(CON12_IN[19-(i-81)]),
        .IO(DC_B[i]),
        .I(CON12_OUT[19-(i-81)]),
        .T(CON12_OEN[19-(i-81)])
      );
    end
end
endgenerate

//========================================================================
//                          Internal modules 
//========================================================================

mc_sig_breakout mc_sig_breakout (

  // Interface to single ended IO buffers
  .CON3_IN(CON3_IN), .CON4_IN(CON4_IN), .CON5_IN(CON5_IN), .CON6_IN(CON6_IN), .CON7_IN(CON7_IN),
  .CON8_IN(CON8_IN), .CON12_IN(CON12_IN),
  .CON3_OEN(CON3_OEN), .CON4_OEN(CON4_OEN), .CON5_OEN(CON5_OEN), .CON6_OEN(CON6_OEN), .CON7_OEN(CON7_OEN),
  .CON8_OEN(CON8_OEN), .CON12_OEN(CON12_OEN),
  .CON3_OUT(CON3_OUT), .CON4_OUT(CON4_OUT), .CON5_OUT(CON5_OUT), .CON6_OUT(CON6_OUT), .CON7_OUT(CON7_OUT),
  .CON8_OUT(CON8_OUT), .CON12_OUT(CON12_OUT),

  // Interface to differential IO buffers  
  .CON17_DIFF_IN(CON17_DIFF_IN), .CON18_DIFF_IN(CON18_DIFF_IN),
  .CON17_DIFF_OEN(CON17_DIFF_OEN), .CON18_DIFF_OEN(CON18_DIFF_OEN),  
  .CON17_DIFF_OUT(CON17_DIFF_OUT), .CON18_DIFF_OUT(CON18_DIFF_OUT),  
  
  // Interfaces to FPGA core
  
  // I2C interface to communicate with the digital potentiometers on the ADC-DAC board
  .SCL_DIR_N(SCL_DIR_N), // 0 == output enabled -> Master (FPGA) to Slave direction
  .SCL_OUT(SCL_OUT),     // From master (FPGA) to slave
  .SDA_DIR_N(SDA_DIR_N), // 0 == output enabled -> Master (FPGA) to Slave direction
  .SDA_OUT(SDA_OUT),     // From master (FPGA) to slave
  .SCL_IN(SCL_IN),       // From slave to master (FPGA)
  .SDA_IN(SDA_IN),       // From slave to master (FPGA)

  // ADC interface
  .ADC_CLK_IN(ADC_CLK_IN), 
  .ADC_CS_IN(ADC_CS_IN),
  .ADC_DATA_OUT(ADC_DATA_OUT),
  
  // DAC interface
  .DAC_CLK_IN(DAC_CLK_IN),
  .DAC_DATA_IN(DAC_DATA_IN),
  .DAC_SYNC_IN(DAC_SYNC_IN),
  
  // Waveform generator interface
  .WF_CLK(WF_CLK),  
  .WF_DATA_IN(WF_DATA_IN),
  .WF_FSYNC_IN(WF_FSYNC_IN),

  // General purpose IOs on ADC board
  .GPCLK_IN(GPCLK_IN),
  .GPIO_IN(GPIO_IN),
  
  // Differential to single ended drivers interface
  .DIFF_IN(DIFF_IN),
  .DIFF_OEN(DIFF_OEN),
  .DIFF_OUT(DIFF_OUT)

);

endmodule
