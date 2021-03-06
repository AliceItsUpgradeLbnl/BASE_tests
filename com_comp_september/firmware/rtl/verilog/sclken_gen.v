//=============================================================================
// Title: SPI clock generator
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 3/17/2016
// Last modified: 3/17/2016 (AC)
//=============================================================================

module sclken_gen(
  input  CLK,                  // System clock
  input  RST_N,                // Active low reset
  input  SCLK,                 // SPI clock	
  output CLK_EN                // Clock enable
);

//========================================================================
//                           Internal Signals 
//========================================================================

reg  enable;
reg  reg_enable;

//========================================================================
//               Counter of the number of main clock ticks 
//========================================================================

always @(negedge CLK or negedge RST_N) begin
  if (!RST_N)
    enable  <= 1'b0;
  else
    enable  <= SCLK;
end

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    reg_enable  <= 1'b0;
  else
    reg_enable  <= enable;
end

assign CLK_EN = reg_enable;

endmodule
