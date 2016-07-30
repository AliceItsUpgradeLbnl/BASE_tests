//=============================================================================
// Title: Interrupt checker satte machine
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 7/23/2016
// Last modified: 7/28/2016 (AC)
// Description: checks whether any transition of the INTERRUPT_BAR line occurs
//              and registers the initial state of that line upon start
//=============================================================================

module interrupt_checker_sm (
  // Clock interface
  input  CLK,                          // System clock
  input  RST_N,                        // Active low reset

  // Start/busy interface
  input      START,
  output reg BUSY,

  // Interrupt and interrupt initial state registered and registered transition
  input      INTERRUPT_BAR,
  output reg INTERRUPT_INIT_STATE,
  output reg INTERRUPT_TRANSITION
);

//========================================================================
//                          State machine states 
//========================================================================

localparam IDLE = 2'b00;
localparam LATCHINITSTATE = 2'b01;
localparam RESETTRANSITIONREG = 2'b10;

//========================================================================
//                             Internal signals
//========================================================================

reg latch_init_state, reset_transition_reg; 
reg [2:0] interrupt_synchronizer;
reg [1:0] sm_state;

//========================================================================
//                            State machine core 
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    sm_state         <= IDLE;
  end else begin
    case (sm_state)
      IDLE: begin
        if (START)
          sm_state   <= LATCHINITSTATE;
        else
          sm_state   <= IDLE;
      end
      LATCHINITSTATE: begin
        sm_state     <= RESETTRANSITIONREG;	
      end
      RESETTRANSITIONREG: begin
        sm_state     <= IDLE;
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
      BUSY                  = 1'b0;
		latch_init_state      = 1'b0;
		reset_transition_reg  = 1'b0;
    end
    LATCHINITSTATE: begin
      BUSY                  = 1'b1;
		latch_init_state      = 1'b1;
		reset_transition_reg  = 1'b0;
    end
    RESETTRANSITIONREG: begin
      BUSY                  = 1'b1;
		latch_init_state      = 1'b0;
		reset_transition_reg  = 1'b1;
    end
    default: begin
      BUSY                  = 1'b0;
		latch_init_state      = 1'b0;
		reset_transition_reg  = 1'b0;
    end
  endcase
end

//========================================================================
//                             Sequential logic 
//========================================================================

// Synchronizer for INTERRUPT_BAR signal
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    interrupt_synchronizer    <= 3'b111; 
  end else begin
    interrupt_synchronizer[2] <= interrupt_synchronizer[1];    
    interrupt_synchronizer[1] <= interrupt_synchronizer[0];
	 interrupt_synchronizer[0] <= INTERRUPT_BAR;
  end
end

// Initial state register
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    INTERRUPT_INIT_STATE <= 1'b0;
  end else if (latch_init_state)
    INTERRUPT_INIT_STATE <= INTERRUPT_BAR;
end

assign interrupt_bar_delayed = interrupt_synchronizer[2];
assign interrupt_bar_undelayed = interrupt_synchronizer[1];
assign int_transition = !interrupt_bar_undelayed && interrupt_bar_delayed;  // Detects falling edge on INTERRUPT_BAR

// Transition register
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    INTERRUPT_TRANSITION <= 1'b0;  
  end else if (reset_transition_reg)
    INTERRUPT_TRANSITION <= 1'b0;
  else if (int_transition)
    INTERRUPT_TRANSITION <= 1'b1;
end

endmodule
