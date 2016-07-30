//=============================================================================
// Title: Pattern checker for differential IO checker
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 7/22/2016
// Last modified: 7/28/2016 (AC)
// Description: checks matching of an input pattern coming from the differential
//              to single ended driver (on DUT board) with an internally
//              generated pseudo-random pattern 
//=============================================================================

module diffio_pattern_checker_sm #(
  parameter NUM_BITS_TO_CHECK = 1000,
  parameter SEED = 32'hABCDEF01
)

(
  // Clock interface
  input  CLK,                                 // System clock (50MHz)
  input  CLK_EN,                              // Clock enable (state machine teansitions allowed when high)
  input  RST_N,                               // Active low reset
  
  // Start/busy interface to main checker state machine
  input  START,                   
  output reg BUSY,           
  
  // Pattern to check and errors found in pattern
  input             BIT_PATTERN,
  output reg [31:0] ERROR_COUNTER
);

//========================================================================
//                            Internal Signals 
//========================================================================

wire expected_bit;
wire last_bit;
wire reset_bit_pattern;
wire rst_bit_counter;
wire rst_error_counter;
wire shift_bit;

reg  [31:0] bit_counter;
reg  bit_is_wrong;
reg  [31:0] d;
reg  incr_bit_counter;
reg  [3:0] sm_state;

//========================================================================
//                          State machine states 
//========================================================================

localparam IDLE = 4'b0000;
localparam CHECKBIT = 4'b0001;
localparam DELAY = 4'b0010;

//========================================================================
//                            State machine core 
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    sm_state         <= IDLE;
  end else if (CLK_EN) begin
    case (sm_state)
      IDLE: begin
        if (START)
          sm_state   <= CHECKBIT;
        else
          sm_state   <= IDLE;
      end
      CHECKBIT: begin
		  sm_state     <= DELAY;	
      end
      DELAY: begin
        if (last_bit)
          sm_state   <= IDLE;
        else
          sm_state   <= CHECKBIT;	
      end
      default: begin
        sm_state     <= IDLE;
      end
    endcase
  end
end

//========================================================================
//                      State machine output signals 
//========================================================================

always@(*) begin
  case (sm_state)
    IDLE: begin
      BUSY                = 1'b0;
      bit_is_wrong        = 1'b0;
      incr_bit_counter    = 1'b0;
    end
    CHECKBIT: begin
      BUSY                = 1'b1;
      bit_is_wrong        = (BIT_PATTERN != expected_bit);
      incr_bit_counter    = 1'b0;
    end
    DELAY: begin
      BUSY                = 1'b1;
      bit_is_wrong        = 1'b0;
      incr_bit_counter    = 1'b1;
    end
    default: begin
      BUSY                = 1'b0;
      bit_is_wrong        = 1'b0;
      incr_bit_counter    = 1'b0;
    end
  endcase
end

//========================================================================
//                             Auxiliary Logic
//========================================================================

// Counter of the number of bits to check

assign last_bit = (bit_counter == NUM_BITS_TO_CHECK - 1'b1);
assign rst_bit_counter = (incr_bit_counter && last_bit);

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    bit_counter   <= 32'd0;
  end else if (CLK_EN) begin
    if (rst_bit_counter)
      bit_counter <= 32'd0;
    else if (incr_bit_counter)
      bit_counter <= bit_counter + 1'b1;
  end
end

// Pseudo-random pattern generator

assign reset_bit_pattern = (incr_bit_counter && last_bit);
assign shift_bit = incr_bit_counter;

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
      d <= SEED;
  else if (CLK_EN) begin
    if (reset_bit_pattern)
      d <= SEED;
    else if (shift_bit) 
      d <= { d[30:0], d[30] ^ d[27] };
  end
end

assign expected_bit = d[31];

// Counter of the number of error bits 

assign rst_error_counter = (START && !BUSY);

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    ERROR_COUNTER   <= 32'd0;
  end else if (CLK_EN) begin
    if (rst_error_counter)
      ERROR_COUNTER <= 32'd0;
    else if (bit_is_wrong)
      ERROR_COUNTER <= ERROR_COUNTER + 1'b1;
  end
end

endmodule
