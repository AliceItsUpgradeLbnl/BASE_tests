//=============================================================================
// Title: Differential IO checker clock divider
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 7/22/2016
// Last modified: 7/28/2016 (AC)
//=============================================================================

module diffio_checker_clock_divider #(
  parameter INPUT_CLK_FREQUENCY = 50000000,   // Input clock frequency
  parameter BASE_FREQUENCY = 125000           // Minimum frequency of CLK_EN signal
)
(
  input  CLK,             // System clock
  input  RST_N,           // Active low reset
  input  [4:0] FREQ_SCALE_FACTOR,
  output reg CLK_EN       // Clock enable
);

//========================================================================
//                           Local parameters 
//========================================================================

localparam DC_TIME_UNIT = INPUT_CLK_FREQUENCY/BASE_FREQUENCY;

//========================================================================
//                           Internal Signals 
//========================================================================

reg  [$clog2(DC_TIME_UNIT) + 1:0] clk_tick_counter;
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
  rst_counter = (clk_tick_counter == (DC_TIME_UNIT/(FREQ_SCALE_FACTOR + 1'b1)) - 1'b1);
  CLK_EN = rst_counter;
end

endmodule
