//=============================================================================
// Title: Differential IO checker main state machine
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 7/23/2016
// Last modified: 7/28/2016 (AC)
// Description: coordinates the execution of the tests on differential IO
//              channels of driver SN65MLVD080DGG
//=============================================================================

module diffio_checker_main_sm (
  // Clock interface
  input       CLK,                          // System clock
  input       RST_N,                        // Active low reset

  // Start/busy interface from outside the Differential IO checker
  input       START,
  output reg  BUSY,

  // Start/busy signals to/from other state machines
  input       BUSY_EVEN,
  input       BUSY_ODD,
  input       [7:0] DATA_TO_SEND, 
  output      [7:0] DATA_TO_CHECK, 
  output reg  START_EVEN_CHANNELS,
  output reg  START_ODD_CHANNELS,
 
  // Differential buffers data/control signals
  input       [7:0] DIFF_IN,
  output      [7:0] DIFF_OEN,
  output      [7:0] DIFF_OUT  
);

//========================================================================
//                          State machine states 
//========================================================================

localparam IDLE = 3'b000;
localparam STARTEVENCHANNELS = 3'b001;   // Starts the generation of a random pattern on even channels
localparam WAITENDEVENCHANNELS = 3'b010;
localparam STARTODDCHANNELS = 3'b011;    // Starts the generation of a random pattern on odd channels
localparam WAITENDODDCHANNELS = 3'b100;

//========================================================================
//                             Internal signals
//========================================================================

reg test_even_channels, test_odd_channels;

reg  [2:0] sm_state;

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
          sm_state   <= STARTEVENCHANNELS;
        else
          sm_state   <= IDLE;
      end
      STARTEVENCHANNELS: begin
        if (BUSY_EVEN)
          sm_state   <= WAITENDEVENCHANNELS;
        else
          sm_state   <= STARTEVENCHANNELS;		
      end
      WAITENDEVENCHANNELS: begin
        if (!BUSY_EVEN)
          sm_state   <= STARTODDCHANNELS;
        else
          sm_state   <= WAITENDEVENCHANNELS;
      end
      STARTODDCHANNELS: begin
        if (BUSY_ODD)
          sm_state   <= WAITENDODDCHANNELS;
        else
          sm_state   <= STARTODDCHANNELS;		
      end
      WAITENDODDCHANNELS: begin
        if (!BUSY_ODD)
          sm_state   <= IDLE;
        else
          sm_state   <= WAITENDODDCHANNELS;	
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
      START_EVEN_CHANNELS   = 1'b0;
      START_ODD_CHANNELS    = 1'b0;
      test_even_channels    = 1'b0;
      test_odd_channels     = 1'b0;
    end
    STARTEVENCHANNELS: begin
      BUSY                  = 1'b1;
      START_EVEN_CHANNELS   = 1'b1;
      START_ODD_CHANNELS    = 1'b0;
      test_even_channels    = 1'b1;
      test_odd_channels     = 1'b0;
    end
    WAITENDEVENCHANNELS: begin
      BUSY                  = 1'b1;
      START_EVEN_CHANNELS   = 1'b0;
      START_ODD_CHANNELS    = 1'b0;
      test_even_channels    = 1'b1;
      test_odd_channels     = 1'b0;
    end
    STARTODDCHANNELS: begin
      BUSY                  = 1'b1;
      START_EVEN_CHANNELS   = 1'b0;
      START_ODD_CHANNELS    = 1'b1;
      test_even_channels    = 1'b0;
      test_odd_channels     = 1'b1;
    end
    WAITENDODDCHANNELS: begin
      BUSY                  = 1'b1;
      START_EVEN_CHANNELS   = 1'b0;
      START_ODD_CHANNELS    = 1'b0;
      test_even_channels    = 1'b0;
      test_odd_channels     = 1'b1;
    end
    default: begin
      BUSY                  = 1'b0;
      START_EVEN_CHANNELS   = 1'b0;
      START_ODD_CHANNELS    = 1'b0;
      test_even_channels    = 1'b0;
      test_odd_channels     = 1'b0;
    end
  endcase
end

//========================================================================
//                           Combinatorial logic 
//========================================================================

// Direction of even channels
assign DIFF_OEN[0] = !test_even_channels;
assign DIFF_OEN[2] = !test_even_channels;
assign DIFF_OEN[4] = !test_even_channels;
assign DIFF_OEN[6] = !test_even_channels;

// Direction of odd channels
assign DIFF_OEN[1] = !test_odd_channels;
assign DIFF_OEN[3] = !test_odd_channels;
assign DIFF_OEN[5] = !test_odd_channels;
assign DIFF_OEN[7] = !test_odd_channels;

assign DATA_TO_CHECK = DIFF_IN;
assign DIFF_OUT = DATA_TO_SEND;

endmodule
