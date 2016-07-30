//=============================================================================
// Title: Serializer block for I2C interface
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 10/16/2015
// Last modified: 10/16/2015 (AC)
// Description: this module serializes the input data and transmits it via the SDA
//              line to a slave device. Control bits from main state machine and
//              actual output data are merged on the SDA line
//=============================================================================

module i2c_serializer (
  // Clock interface
  input  CLK,                        // System clock
  input  CLK_EN,                     // I2C Clock enable
  input  RST_N,                      // Active low reset

  // Input register interface
  input  [63:0] DATA_FROM_CORE,      // Data to transmit to a slave device
  input  LATCH_CORE_SIDE_REG,        // Latch data from core side to core side reg
  
  // Shiftreg interface
  input  LOAD_SHIFTREG,              // Load shift register with a byte from core
  input  SHIFT_OUT,                  // Shift output enable (a data bit goes to the SDA line)

  // I2C input signals from state machine (to be complemented with data and filtered)
  input  SDA_SEL_SM,                 // Used to multiplex SDA state from main sm (control) and this serializer (actual data) 
  input  SCL_SM,                     // I2C clock line from main state machine 
  input  SDA_DIR_SM,                 // SDA line direction
  input  SDA_OUT_SM,                 // SDA line data (from main state machine)

  // I2C output signals to slave device 
  output reg SCL,                    // I2C clock line, glitch filtered
  output reg SDA_DIR,                // SDA line direction signal (high = master to slave). Glitch filtered
  output reg SDA_OUT                 // I2C output data line. Merged data and control bits. Glitch filtered 
);

//========================================================================
//                            Internal Signals 
//========================================================================

reg  [63:0] core_data;
reg  [7:0] data_out;
wire sda_out_int;

//========================================================================
//                        Core side 8-bit registers 
//========================================================================

generate
genvar i;
for (i = 0; i < 7; i = i + 1) begin: core_to_slave_reg
  always @(posedge CLK or negedge RST_N) begin
    if (!RST_N) begin
      core_data[(i+1)*8 - 1:i*8]     <= 8'd0;
    end else if (CLK_EN) begin	
      if (LATCH_CORE_SIDE_REG)
        core_data[(i+1)*8 - 1:i*8]   <= DATA_FROM_CORE[(i+1)*8 - 1:i*8];
      else if (LOAD_SHIFTREG)
        core_data[(i+1)*8 - 1:i*8]   <= core_data[(i+2)*8 - 1:(i+1)*8];
    end
  end
end
endgenerate

always @(posedge CLK or negedge RST_N) begin: core_to_slave_reg_7
  if (!RST_N) begin
    core_data[63:56]     <= 8'd0;
  end else if (CLK_EN) begin
    if (LATCH_CORE_SIDE_REG)
      core_data[63:56]   <= DATA_FROM_CORE[63:56];
    else if (LOAD_SHIFTREG)
      core_data[63:56]   <= 8'd0;
  end
end

//========================================================================
//    Ouput data shift register (DATA_FROM_CORE[7:0] -> data_out[7])
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    data_out        <= 8'd0;
  end else if (CLK_EN) begin
    if (LOAD_SHIFTREG)
      data_out      <= core_data[7:0];
    else if (SHIFT_OUT) begin
      data_out[7:1] <= data_out[6:0];
      data_out[0]   <= 1'b0;
    end
  end
end

//========================================================================
//                        Output data multiplexer
//========================================================================

assign sda_out_int = SDA_SEL_SM ? SDA_OUT_SM : data_out[7];

//========================================================================
//                Glitch filter (SDA_OUT, SCL and SDA_DIR)
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    SCL          <= 1'b1;
    SDA_DIR      <= 1'b0;
    SDA_OUT      <= 1'b1;
  end else begin
    SCL          <= SCL_SM;
    SDA_DIR      <= SDA_DIR_SM;
    SDA_OUT      <= sda_out_int;
  end
end

endmodule
