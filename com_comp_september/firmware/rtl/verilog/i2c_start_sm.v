//=============================================================================
// Title: I2C Start state machine and busy generation
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 10/30/2015
// Last modified: 10/30/2015 (AC)
// Description: this state machine generates a start signal for the main state
//              machine of the I2C interface (it stretches the external start
//              signal to make it "visible" to the main I2C state machine)
//=============================================================================

module i2c_start_sm (
  // Clock interface
  input  CLK,                                 // System clock (50 MHz)
  input  RST_N,                               // Active low reset

  // Start signal from external control interface
  input  START_I2C,

  // Signals from/to main I2C SM 
  input  BUSY_MAIN_SM,                        // Busy signal from main SM
  output reg BUSY,                            // Busy signal to external control interface
  output reg START_MAIN_SM                    // Start signal to main SM
);

//========================================================================
//                            Internal signals 
//========================================================================

reg [1:0] sm_state;

//========================================================================
//                          State machine states 
//========================================================================

localparam IDLE = 2'b00;
localparam START = 2'b01;
localparam BUSYHIGH = 2'b10;

//========================================================================
//                            State machine core 
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    sm_state         <= IDLE;
  end else begin
    case (sm_state)
      IDLE: begin
	if (START_I2C)
          sm_state   <= START;
        else
          sm_state   <= IDLE;		  
      end
      START: begin
	if (BUSY_MAIN_SM)
          sm_state   <= BUSYHIGH;
        else
          sm_state   <= START;
      end
      BUSYHIGH: begin
	if (!BUSY_MAIN_SM)
          sm_state   <= IDLE;
        else
          sm_state   <= BUSYHIGH;
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
      BUSY           = 1'b0;
      START_MAIN_SM  = 1'b0;
    end
    START: begin
      BUSY           = 1'b1;
      START_MAIN_SM  = 1'b1;
    end
    BUSYHIGH: begin
      BUSY           = 1'b1;
      START_MAIN_SM  = 1'b0;
    end
    default: begin
      BUSY           = 1'b0;
      START_MAIN_SM  = 1'b0;
    end
  endcase
end

endmodule
