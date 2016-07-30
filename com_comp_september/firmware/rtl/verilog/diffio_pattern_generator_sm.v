//=============================================================================
// Title: Pattern generator for differential IO checker
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 7/22/2016
// Last modified: 7/22/2016 (AC)
// Description: generates a pattern that is provided to a channel of the
//              differential to single ended driver SN65MLVD080DGG (on DUT board)
//=============================================================================

module diffio_pattern_generator_sm #(
  parameter NUM_BITS_TO_SEND = 1000,
  parameter SEED = 32'hABCDEF01
)
(
  // Clock interface
  input  CLK,                                 // System clock (50MHz)
  input  CLK_EN,                              // Clock enable (state machine teansitions allowed when high)
  input  RST_N,                               // Active low reset
  
  // Start/busy interface
  input  START,                               // Start an operation
  output reg BUSY,                            // Indicates transaction in progress
  
  // Generated pattern
  output BIT_PATTERN
);

//========================================================================
//                            Internal Signals 
//========================================================================

wire last_bit;
wire reset_bit_pattern;
wire rst_bit_counter;
wire shift_bit;

reg  [31:0] bit_counter;
reg  [31:0] d;
reg  incr_bit_counter;
reg  [3:0] sm_state;

//========================================================================
//                          State machine states 
//========================================================================

localparam IDLE = 4'b0000;
localparam BITASSERTION = 4'b0001;
localparam BITDEASSERTION = 4'b0010;

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
          sm_state   <= BITASSERTION;
        else
          sm_state   <= IDLE;
      end
      BITASSERTION: begin
		  sm_state     <= BITDEASSERTION;	
      end
      BITDEASSERTION: begin
	     if (last_bit)
          sm_state   <= IDLE;
        else
          sm_state   <= BITASSERTION;	
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
      incr_bit_counter    = 1'b0;
    end
    BITASSERTION: begin
      BUSY                = 1'b1;
      incr_bit_counter    = 1'b0;
    end
    BITDEASSERTION: begin
      BUSY                = 1'b1;
      incr_bit_counter    = 1'b1;
    end
    default: begin
      BUSY                = 1'b0;
      incr_bit_counter    = 1'b0;
    end
  endcase
end

//========================================================================
//                             Auxiliary Logic
//========================================================================

// Counter of the number of bits to generate

assign last_bit = (bit_counter == NUM_BITS_TO_SEND - 1'b1);
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

// Pattern generator

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

assign BIT_PATTERN = d[31];

endmodule
