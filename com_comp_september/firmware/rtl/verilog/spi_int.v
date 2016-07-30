//=============================================================================
// Title: SPI Interface
// Author: Joanna Szornel
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 2/29/2016
// Last modified: 7/28/2016 (AC)
// Description: generic SPI interface written to communicate with SPI
//              devices on the DUT and ADC boards
//=============================================================================

module spi_int(
  input  SCLK,                                 // 50 MHz clock
  input  START_SM,                             // Initiate transaction
  input  RST_N,                                // Reset state machine (active low)
  input  [1:0] OPT_TYPE,                       // Operation type (0 W, 1 WR, 2 R)
  input  [4:0] NUM_BITS_TO_READ,               // Number of bytes to be recieved from a slave device
  input  [4:0] NUM_BITS_TO_WRITE,              // Number of bytes to be tramsmitted to a slave device
  input  MISO,
  input  [23:0] DATA_TO_SLAVE,                 // 24 bit register

  input  SCLK_EN,                              // 25 MHz

  output reg SS_b,                             // Slave select
  output MOSI,
  output reg [23:0] DATA_FROM_SLAVE,
  output reg BUSY                              // Indicates a transaction is in progress
);

//========================================================================
//                          Internal Signals
//========================================================================

reg read_counter_inc;                         // increment read counter
reg read_counter_reset;                       // reset read counter

reg write_counter_inc;                        // increment write counter
reg write_counter_reset;                      // reset write counter

reg shiftreg_to_slave_load;
reg shiftreg_to_slave_shift;

reg shiftreg_from_slave_load;
reg shiftreg_from_slave_shift;

reg [23:0] shiftreg_to_slave_24;
reg [15:0] shiftreg_to_slave_16;
reg [7:0] shiftreg_to_slave_8;
reg [23:0] shiftreg_from_slave;

reg [4:0] read_counter;
reg [4:0] write_counter;

reg [2:0] sm_state;

//========================================================================
//                         State Machine States
//========================================================================

localparam IDLE = 3'b000;
localparam LOAD = 3'b001;
localparam WRITE = 3'b010;
localparam READ = 3'b011;
localparam STOP = 3'b100;

//========================================================================
//                          State Machine Core
//========================================================================

always @(posedge SCLK or negedge RST_N) begin
  if (!RST_N) begin
    sm_state         <= IDLE;
  end
  else if (SCLK_EN) begin
    case (sm_state)
      IDLE: begin
        if (START_SM)
          sm_state   <= LOAD;
        else
          sm_state   <= IDLE;
        end
      LOAD: begin
        if (OPT_TYPE == (2'b00))
          sm_state <= WRITE;
        else if (OPT_TYPE == (2'b01))
          sm_state <= WRITE;
        else if (OPT_TYPE == (2'b10))
          sm_state <= READ;
        else
          sm_state <= LOAD;
      end
      WRITE: begin
        if (write_counter == (NUM_BITS_TO_WRITE -1'b1)) begin
          if (OPT_TYPE == 2'b00)
            sm_state   <= STOP;
          else if (OPT_TYPE == 2'b01)
            sm_state   <= READ;
        end else
          sm_state   <= WRITE;
        end
      READ: begin
        if (read_counter == (NUM_BITS_TO_READ -1'b1))
          sm_state   <= STOP;
        else
          sm_state   <= READ;
      end
      STOP: begin
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

always @(*) begin
  case (sm_state)
    IDLE: begin
      BUSY                      = 1'b0;
      read_counter_inc          = 1'b0;
      read_counter_reset        = 1'b0;
      shiftreg_to_slave_load    = 1'b0;
      shiftreg_to_slave_shift   = 1'b0;
      write_counter_inc         = 1'b0;
      write_counter_reset       = 1'b0;
      shiftreg_from_slave_load  = 1'b0;
      shiftreg_from_slave_shift = 1'b0;
      SS_b                      = 1'b1;
    end
    LOAD: begin
      BUSY                      = 1'b1;
      read_counter_inc          = 1'b0;
      read_counter_reset        = 1'b0;
      shiftreg_to_slave_load    = 1'b1;
      shiftreg_to_slave_shift   = 1'b0;
      write_counter_inc         = 1'b0;
      write_counter_reset       = 1'b0;
      shiftreg_from_slave_load  = 1'b0;
      shiftreg_from_slave_shift = 1'b0;
      SS_b                      = 1'b1;
    end
    WRITE: begin
      BUSY                      = 1'b1;
      read_counter_inc          = 1'b0;
      read_counter_reset        = 1'b0;
      shiftreg_to_slave_load    = 1'b0;
      shiftreg_to_slave_shift   = 1'b1;
      write_counter_inc         = 1'b1;
      write_counter_reset       = 1'b0;
      shiftreg_from_slave_load  = 1'b0;
      shiftreg_from_slave_shift = 1'b0;
      SS_b                      = 1'b0;
    end
    READ: begin
      BUSY                      = 1'b1;
      read_counter_inc          = 1'b1;
      read_counter_reset        = 1'b0;
      shiftreg_to_slave_load    = 1'b0;
      shiftreg_to_slave_shift   = 1'b0;
      write_counter_inc         = 1'b0;
      write_counter_reset       = 1'b0;
      shiftreg_from_slave_load  = 1'b0;
      shiftreg_from_slave_shift = 1'b1;
      SS_b                      = 1'b0;
    end
    STOP: begin
      BUSY                      = 1'b1;
      read_counter_inc          = 1'b0;
      read_counter_reset        = 1'b1;
      shiftreg_to_slave_load    = 1'b0;
      shiftreg_to_slave_shift   = 1'b0;
      write_counter_inc         = 1'b0;
      write_counter_reset       = 1'b1;
      shiftreg_from_slave_load  = 1'b1;
      shiftreg_from_slave_shift = 1'b0;
      SS_b                      = 1'b1;
    end
    default: begin
      BUSY                      = 1'b0;
      read_counter_inc          = 1'b0;
      read_counter_reset        = 1'b0;
      shiftreg_to_slave_load    = 1'b0;
      shiftreg_to_slave_shift   = 1'b0;
      write_counter_inc         = 1'b0;
      write_counter_reset       = 1'b0;
      shiftreg_from_slave_load  = 1'b0;
      shiftreg_from_slave_shift = 1'b0;
      SS_b                      = 1'b1;
    end
  endcase
end

//========================================================================
//                  Shift Register - Serial to Parallel
//========================================================================

always @(posedge SCLK or negedge RST_N) begin
  if (!RST_N) begin
    shiftreg_from_slave[23:0] <= 24'd0;
  end
  else if (SCLK_EN) begin
    if (shiftreg_from_slave_shift) begin
      shiftreg_from_slave[23:1] <= shiftreg_from_slave[22:0];
      shiftreg_from_slave[0]    <= MISO;
    end
  end
end

//========================================================================

always @(posedge SCLK or negedge RST_N) begin
  if (!RST_N) begin
    DATA_FROM_SLAVE[23:0]   <= 24'd0;
  end
  else if (SCLK_EN) begin
    if (shiftreg_from_slave_load) begin       // happens in "LOAD" state
      DATA_FROM_SLAVE[23:0] <= shiftreg_from_slave[23:0];
    end
  end
end

//========================================================================
//                  Shift Register - Parallel to Serial
//========================================================================


always @(posedge SCLK or negedge RST_N) begin
  if (!RST_N) begin
    shiftreg_to_slave_24[23:0]   <= 24'd0;
    shiftreg_to_slave_16[15:0] <= 16'd0;
    shiftreg_to_slave_8[7:0] <= 8'd0;
  end
  else if (SCLK_EN) begin
    if (write_counter_reset || read_counter_reset) begin
      shiftreg_to_slave_24[23:0]   <= 24'd0;
      shiftreg_to_slave_16[15:0] <= 16'd0;
      shiftreg_to_slave_8[7:0] <= 8'd0;
    end
    else if (shiftreg_to_slave_load) begin    // happens in "LOAD" state
      if (NUM_BITS_TO_WRITE == 5'd24) begin
        shiftreg_to_slave_24[23:0] <= DATA_TO_SLAVE[23:0];
      end
      else if (NUM_BITS_TO_WRITE == 5'd16) begin
        shiftreg_to_slave_16[15:0] <= DATA_TO_SLAVE[23:8];
      end
      else if (NUM_BITS_TO_WRITE == 5'd8) begin
        shiftreg_to_slave_8[7:0] <= DATA_TO_SLAVE[23:16];
      end
    end
    else if (shiftreg_to_slave_shift) begin
      if (NUM_BITS_TO_WRITE == 5'd24) begin
        shiftreg_to_slave_24[23:1] <= shiftreg_to_slave_24[22:0];
      end
      else if (NUM_BITS_TO_WRITE == 5'd16) begin
        shiftreg_to_slave_16[15:1] <= shiftreg_to_slave_16[14:0];
      end
      else if (NUM_BITS_TO_WRITE == 5'd8) begin
        shiftreg_to_slave_8[7:1] <= shiftreg_to_slave_8[6:0];
      end
    end
  end
end

assign MOSI = shiftreg_to_slave_24[23] || shiftreg_to_slave_16[15] || shiftreg_to_slave_8[7];

//========================================================================
//                       counter for bits written
//========================================================================

always @(posedge SCLK or negedge RST_N) begin
  if (!RST_N) begin
    write_counter <= 5'd0;
  end
  else if (SCLK_EN) begin
    if (write_counter_reset) begin
      write_counter <= 5'd0;
    end
    else if (write_counter_inc) begin
      write_counter <= write_counter + 1'b1;
    end
  end
end

//========================================================================
//                       counter for bits read
//========================================================================

always @(posedge SCLK or negedge RST_N) begin
  if (!RST_N) begin
    read_counter <= 5'd0;
  end
  else if (SCLK_EN) begin
    if (read_counter_reset) begin
      read_counter <= 5'd0;
    end
    else if (read_counter_inc) begin
      read_counter <= read_counter + 1'b1;
    end
  end
end

endmodule
