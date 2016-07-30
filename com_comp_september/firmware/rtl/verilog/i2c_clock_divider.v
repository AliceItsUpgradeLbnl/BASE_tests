//=============================================================================
// Title: I2C clock divider
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 10/16/2015
// Last modified: 10/16/2015 (AC)
// Description: generates a clock enable signal for the I2C interface that can
//              be used to reduce its speed (bitrate on SDA and SCL lines).
//              The SDA and SCL frequency can be set at synthesis time only.    
//=============================================================================

module i2c_clock_divider #(
  parameter INPUT_CLK_FREQUENCY = 50000000,   // Input clock frequency (req: INPUT_CLK_FREQUENCY/BUS_CLK_FREQUENCY >= 20)
  parameter BUS_CLK_FREQUENCY = 100000        // I2C SCL frequency (fixed duty ratio = 33%)
)
(
  input      CLK,              // System clock
  input      RST_N,            // Active low reset
  output reg CLK_EN            // Clock enable
);

//========================================================================
//                           Local parameters 
//========================================================================

localparam I2C_TIME_UNIT = (INPUT_CLK_FREQUENCY/BUS_CLK_FREQUENCY/3);

//========================================================================
//                           Internal Signals 
//========================================================================

reg  [$clog2(I2C_TIME_UNIT) + 1:0] clk_tick_counter;
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
  rst_counter = (clk_tick_counter == I2C_TIME_UNIT - 1'b1);
  CLK_EN = rst_counter;
end

endmodule
