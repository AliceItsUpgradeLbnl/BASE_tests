//=============================================================================
// Title: Slow Clock Generator for SPI
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 3/17/2016
// Last modified: 3/17/2016 (AC)
//=============================================================================

module slow_clock_spi #(
  parameter INPUT_CLK_FREQUENCY = 50000000,      // Input clock frequency
  parameter OUTPUT_CLK_FREQUENCY = 2500000       // Output clock frequency
)
(
  input  CLK,                  // System clock
  input  RST_N,                // Active low reset
  output reg CLK_OUT           // Clock output
);

//========================================================================
//                           Local parameters 
//========================================================================

localparam TIME_UNIT = (INPUT_CLK_FREQUENCY/OUTPUT_CLK_FREQUENCY/2);

//========================================================================
//                           Internal Signals 
//========================================================================

reg  [$clog2(TIME_UNIT) + 1:0] clk_tick_counter;
reg  rst_counter;

//========================================================================
//               Counter of the number of main clock ticks 
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    clk_tick_counter   <= 0;
  end else begin
    if (rst_counter)
      clk_tick_counter <= 0;
    else
      clk_tick_counter <= clk_tick_counter + 1'b1;
  end
end

always @(*) begin
  rst_counter = (clk_tick_counter == TIME_UNIT - 1'b1);
end

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    CLK_OUT   <= 0;
  else if (rst_counter)
    CLK_OUT   <= !CLK_OUT;
end

endmodule