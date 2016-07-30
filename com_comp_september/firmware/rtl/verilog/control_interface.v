//=============================================================================
// Title: Control interface
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 3/18/2016
// Last modified: 3/18/2016 (AC)
// Description: decodes commands sent from PC via USB and sends back data 
//=============================================================================

module control_interface (
  // Clock interface
  input RESET,
  input CLK,

  // From FPGA to PC
  output [35:0] FIFO_Q,
  output FIFO_EMPTY,
  input FIFO_RDREQ,
  input FIFO_RDCLK,

  // From PC to FPGA
  input [35:0] CMD_FIFO_Q,
  input CMD_FIFO_EMPTY,
  output reg CMD_FIFO_RDREQ,

  // Radiation block interface
  input BUSY_RT,
  input [31:0] FIFO_DATA,
  input FIFO_WREN,
  output FIFO_FULL,
  input [15:0] REG_DATA,
  output reg REG_WREN

);

//==================================================================================================
//                                          Internal Signals
//==================================================================================================

// Signals for FIFO
reg  [35:0] sFifoD;
wire sFifoFull;
reg  sFifoWrreq;
wire sFifoWrreqOR;
wire sFifoRst;
wire sFifoClk;

// Command from PC
wire [11:0] address;
wire op_type;
wire [15:0] data;

// State machine reg
reg [2:0] sm_state;

//==================================================================================================
//                                       Combinatorial logic
//==================================================================================================

assign FIFO_FULL = sFifoFull; 
assign sFifoWrreqOR = (sFifoWrreq || FIFO_WREN);
assign op_type = CMD_FIFO_Q[31];

always @(*) begin
  if (FIFO_WREN)
    sFifoD = {4'd0,FIFO_DATA};
  else
    sFifoD = {20'd0,REG_DATA};
end

assign sFifoRst = RESET;
assign sFifoClk = !CLK;

//========================================================================
//                          State machine states 
//========================================================================

localparam IDLE = 3'b000;
localparam WAITCMD = 3'b001;
localparam GETCMD = 3'b010;
localparam INTERPRETCMD = 3'b011;
localparam WAITOPSTART = 3'b100;
localparam WAITOPEND = 3'b101;

//========================================================================
//                            State machine core 
//========================================================================

assign address = CMD_FIFO_Q[27:16];

always @(posedge CLK or posedge RESET) begin
  if (RESET) begin
    sm_state           <= IDLE;
  end else begin
    case (sm_state)
      IDLE: begin
        sm_state       <= WAITCMD;	  
      end
      WAITCMD: begin
        if (!CMD_FIFO_EMPTY)
          sm_state     <= GETCMD;  
        else 
          sm_state     <= WAITCMD;  
      end
      GETCMD: begin
        sm_state       <= INTERPRETCMD;
      end
      INTERPRETCMD: begin
	if (op_type) begin
          sm_state     <= WAITCMD;
        end else begin
          if ((address >= 12'd64) && (address <= 12'd1023)) begin
            if (address == 12'd128)
              sm_state <= WAITOPSTART;
            else
              sm_state <= WAITCMD;
          end else
            sm_state   <= WAITCMD;
        end
      end
      WAITOPSTART: begin
        sm_state       <= WAITOPEND;
      end
      WAITOPEND: begin
        if (BUSY_RT)
          sm_state     <= WAITOPEND;
        else
          sm_state     <= WAITCMD;          
      end
      default: begin
        sm_state       <= IDLE;
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
      CMD_FIFO_RDREQ = 1'b0;
      REG_WREN       = 1'b0;
      sFifoWrreq     = 1'b0;
    end
    WAITCMD: begin
      CMD_FIFO_RDREQ = 1'b0;
      REG_WREN       = 1'b0;
      sFifoWrreq     = 1'b0;
    end
    GETCMD: begin
      CMD_FIFO_RDREQ = 1'b1;
      REG_WREN       = 1'b0;
      sFifoWrreq     = 1'b0;
    end
    INTERPRETCMD: begin
      CMD_FIFO_RDREQ = 1'b0;
      if (op_type) begin // read operation
        sFifoWrreq   = 1'b1;
        REG_WREN     = 1'b0;
      end else begin
        sFifoWrreq   = 1'b0;
        REG_WREN     = 1'b1;
      end
    end
    WAITOPSTART: begin
      CMD_FIFO_RDREQ = 1'b0;
      REG_WREN       = 1'b0;
      sFifoWrreq     = 1'b0;
    end
    WAITOPEND: begin
      CMD_FIFO_RDREQ = 1'b0;
      REG_WREN       = 1'b0;
      sFifoWrreq     = 1'b0;
    end
    default: begin
      CMD_FIFO_RDREQ = 1'b0;
      REG_WREN       = 1'b0;
      sFifoWrreq     = 1'b0;
    end
  endcase
end



//====================================================================================================
//                                          Internal Modules
//====================================================================================================

fifo16384x36 fifo16384x36(
  .rst(sFifoRst),
  .wr_clk(sFifoClk),
  .rd_clk(FIFO_RDCLK),
  .din(sFifoD),
  .wr_en(sFifoWrreqOR),
  .rd_en(FIFO_RDREQ),
  .dout(FIFO_Q),
  .full(sFifoFull),
  .empty(FIFO_EMPTY)
);

endmodule

