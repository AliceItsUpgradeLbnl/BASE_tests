//=============================================================================
// Title: Deserializer block for I2C interface
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 10/16/2015
// Last modified: 10/16/2015 (AC)
// Description: this module deserializes the data that is sent by the slave
//              devices via the SDA line and writes this data to a core register.
//=============================================================================

module i2c_deserializer (
  // Clock interface
  input  CLK,                      // System clock
  input  CLK_EN,                   // I2C Clock enable
  input  RST_N,                    // Active low reset
  
  // Control interface to internal shift register and core register
  input  LATCH_CORE_SIDE_REG,
  input  RST_CORE_SIDE_REG,
  input  SHIFT_IN,       

  // Serial data from slave device 
  input  SDA_IN,            

  // Slave device deserialized data (max 31 bytes)
  output reg [247:0] I2C_DATA_REC 
);

//========================================================================
//                           Internal Signals 
//=======================================================================

reg  SDA_IN_int;
reg  [7:0] data_to_core;

//========================================================================
//             Glitch filter on SDA line (SDA_IN -> SDA_IN_int)
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    SDA_IN_int   <= 1'b1;
  end else begin
    SDA_IN_int   <= SDA_IN;
  end
end

//========================================================================
//          Input shift register (SDA_IN_int -> data_to_core[7:0])
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    data_to_core        <= 8'd0;
  end else if (CLK_EN) begin
    if (SHIFT_IN) begin
      data_to_core[7:1] <= data_to_core[6:0];
      data_to_core[0]   <= SDA_IN_int;
    end
  end
end

//========================================================================
//                        Core side register
//========================================================================

always @(posedge CLK or negedge RST_N) begin: slave_to_core_reg_0
  if (!RST_N) begin
    I2C_DATA_REC[7:0]   <= 8'd0;
  end else if (CLK_EN) begin
    if (RST_CORE_SIDE_REG)
      I2C_DATA_REC[7:0] <= 8'd0;		  
    else if (LATCH_CORE_SIDE_REG)
      I2C_DATA_REC[7:0] <= data_to_core;
  end
end

genvar i;

generate for (i = 1; i < 31; i = i + 1) begin: slave_to_core_reg
  always @(posedge CLK or negedge RST_N) begin
    if (!RST_N) begin
      I2C_DATA_REC[(i+1)*8 - 1:i*8]   <= 8'd0;
    end else if (CLK_EN) begin
	   if (RST_CORE_SIDE_REG)
        I2C_DATA_REC[(i+1)*8 - 1:i*8] <= 8'd0;		  
      else if (LATCH_CORE_SIDE_REG)
        I2C_DATA_REC[(i+1)*8 - 1:i*8] <= I2C_DATA_REC[i*8 - 1:(i-1)*8];
    end
  end
end endgenerate

endmodule
