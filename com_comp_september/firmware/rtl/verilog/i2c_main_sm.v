//=============================================================================
// Title: I2C Main state machine
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 10/16/2015
// Last modified: 10/16/2015 (AC)
// Description: this module handles the execution of data serialization, bus
//              turnaround and data deserialization operations on the SDA line
//=============================================================================

module i2c_main_sm (
  // Clock interface
  input  CLK,                         // System clock (50 Mhz)
  input  CLK_EN,                      // Clock enable
  input  RST_N,                       // Active low reset
  
  // Main SM interface
  input  START_SM,                    // Start signal for state machine
  input  REP_START,                   // Repeate start during read operation
  input  RW,                          // READ or !WRITE
  input  [4:0] NUM_BYTES_TO_REC,      // Number of bytes to receive from a slave device
  input  [2:0] NUM_BYTES_TO_TRANSM,   // Number of bytes to transmit to a slave device (the slave address is not included in this)
  output reg BUSY,                    // Indicates transaction in progress

  // Interface to serializer
  output reg LATCH_FROM_CORE,         // Latches data to the serializer core side reg (data will go to a slave device)
  output reg LOAD_OUTPUT_SHIFTREG,    // Loads a data byte from the serializer core side register to the serializer output shiftreg
  output SHIFT_OUT,                   // Shifts data in the serializer (data bit goes out to SDA line)
  
  // Interface to deserializer
  output LATCH_TO_CORE,               // Latches data bytes received from a slave device to the deserializer core side register
  output RST_CORE_SIDE_REG,           // Resets the deserializer core side register
  output SHIFT_IN,                    // Shifts data in the deserializer shift reg (data bit comes in from SDA line)
  
  // I2C output signals (signals glitch filtered outside this sm)
  output reg SDA_SEL_SM,              // Signals that a valid SDA_OUT is being transmitted by the SM
  output reg SCL_SM,                  // I2C clock line
  output reg SDA_DIR_SM,              // Direction of I2C data line
  output reg SDA_OUT_SM               // I2C SDA line (only control part: start, stop, etc, actual data ANDed elsewhere)
);
		
//========================================================================
//                          Internal Signals 
//========================================================================
		
wire incr_bit_counter;
wire rst_bit_counter;
wire last_transm_byte;
wire incr_transm_byte_counter;
wire rst_transm_byte_counter;
wire last_read_byte;
wire incr_read_byte_counter;
wire shift_in;
wire shiftout;
wire last_bit;
wire shift_out;
wire [7:0] data_out;
wire [2:0] data_to_output_shiftreg;
wire latch_data_from_input_shiftreg;
wire [7:0] data_in;
wire rst_read_byte_counter;
wire last_databit;

reg  [2:0] bit_counter;
reg  [4:0] read_byte_counter;
reg  [3:0] transm_byte_counter;

reg  end_of_ack;
reg  end_of_databit;
reg  load_first_b;
reg  read_phase;
reg  rep_start_reg;
reg  scl_high;
reg  start_phase;
reg  transm_phase;
reg  rep_write_seq;

reg [4:0] sm_state;

//========================================================================
//                          State machine states 
//========================================================================

localparam IDLE = 5'b00000;
localparam START = 5'b00001;
localparam SCLLOW = 5'b00010;
localparam TRANSMBITPHASE0 = 5'b00011;
localparam TRANSMBITPHASE1 = 5'b00100;
localparam TRANSMBITPHASE2 = 5'b00101;
localparam RECACKPHASE0 = 5'b00110;
localparam RECACKPHASE1 = 5'b00111;
localparam RECACKPHASE2 = 5'b01000;
localparam REPSTARTPHASE0 = 5'b01001;
localparam REPSTARTPHASE1 = 5'b01010;
localparam REPSTARTPHASE2 = 5'b01011;
localparam RECBITPHASE0 = 5'b01100;
localparam RECBITPHASE1 = 5'b01101;
localparam RECBITPHASE2 = 5'b01110;
localparam TRANSMACKPHASE0 = 5'b01111;
localparam TRANSMACKPHASE1 = 5'b10000;
localparam TRANSMACKPHASE2 = 5'b10001;
localparam BOTHLOW = 5'b10010;
localparam SCLHIGH = 5'b10011;
localparam STOP = 5'b10100;

//========================================================================
//                            State machine core 
//========================================================================

always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    sm_state         <= IDLE;
  end else if (CLK_EN) begin
    case (sm_state)
      IDLE: begin
	if (START_SM)
          sm_state   <= START;
        else
          sm_state   <= IDLE;		  
      end
      START: begin
        sm_state     <= SCLLOW;
      end
      SCLLOW: begin
        sm_state     <= TRANSMBITPHASE0;
      end
      TRANSMBITPHASE0: begin
        sm_state     <= TRANSMBITPHASE1;
      end
      TRANSMBITPHASE1: begin
        sm_state     <= TRANSMBITPHASE2;
      end
      TRANSMBITPHASE2: begin
        if (last_databit)
          sm_state   <= RECACKPHASE0;
        else
          sm_state   <= TRANSMBITPHASE0;
      end
      RECACKPHASE0: begin
        sm_state     <= RECACKPHASE1;
      end
      RECACKPHASE1: begin
        sm_state     <= RECACKPHASE2;
      end
      RECACKPHASE2: begin
        if (last_transm_byte && rep_start_reg)
          sm_state   <= REPSTARTPHASE0;
        else if (last_transm_byte && !RW)
          sm_state   <= BOTHLOW;
        else if (last_transm_byte)
          sm_state   <= RECBITPHASE0;
        else
          sm_state   <= TRANSMBITPHASE0;
      end
      REPSTARTPHASE0: begin
        sm_state     <= REPSTARTPHASE1;
      end
      REPSTARTPHASE1: begin
        sm_state     <= REPSTARTPHASE2;
      end
      REPSTARTPHASE2: begin
        sm_state     <= SCLLOW;
      end
      RECBITPHASE0: begin
        sm_state     <= RECBITPHASE1;
      end
      RECBITPHASE1: begin
        sm_state     <= RECBITPHASE2;
      end
      RECBITPHASE2: begin
        if (last_databit)
          sm_state   <= TRANSMACKPHASE0;
        else
          sm_state   <= RECBITPHASE0;
      end
      TRANSMACKPHASE0: begin
        sm_state     <= TRANSMACKPHASE1;
      end
      TRANSMACKPHASE1: begin
        sm_state     <= TRANSMACKPHASE2;
      end
      TRANSMACKPHASE2: begin
        if (last_read_byte)
          sm_state   <= BOTHLOW;
        else
          sm_state   <= RECBITPHASE0;
      end
      BOTHLOW: begin
        sm_state     <= SCLHIGH;
      end
      SCLHIGH: begin
        sm_state     <= STOP;
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

always@(*) begin
  case (sm_state)
    IDLE: begin
      BUSY           = 1'b0;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b0;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    START: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b0;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b1;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    SCLLOW: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b1;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      SDA_OUT_SM     = 1'b0;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    TRANSMBITPHASE0: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b0;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b1;
      rep_write_seq  = 1'b0;
    end
    TRANSMBITPHASE1: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b0;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b1;
      rep_write_seq  = 1'b0;
    end
    TRANSMBITPHASE2: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b1;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b0;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b1;
      rep_write_seq  = 1'b0;
    end
    RECACKPHASE0: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b0;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    RECACKPHASE1: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b0;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    RECACKPHASE2: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b1;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b0;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    REPSTARTPHASE0: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b1;
    end
    REPSTARTPHASE1: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    REPSTARTPHASE2: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b0;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    RECBITPHASE0: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b0;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b1;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    RECBITPHASE1: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b0;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b1;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    RECBITPHASE2: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b1;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b0;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b1;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    TRANSMACKPHASE0: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      if (last_read_byte) begin
        SDA_OUT_SM   = 1'b1;
      end else begin
        SDA_OUT_SM   = 1'b0;
      end
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    TRANSMACKPHASE1: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      if (last_read_byte) begin
        SDA_OUT_SM   = 1'b1;
      end else begin
        SDA_OUT_SM   = 1'b0;
      end
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    TRANSMACKPHASE2: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b1;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      if (last_read_byte) begin
        SDA_OUT_SM   = 1'b1;
      end else begin
        SDA_OUT_SM   = 1'b0;
      end
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    BOTHLOW: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b0;
      SCL_SM         = 1'b0;
      SDA_OUT_SM     = 1'b0;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    SCLHIGH: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b0;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    STOP: begin
      BUSY           = 1'b1;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b1;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
    default: begin
      BUSY           = 1'b0;
      end_of_ack     = 1'b0;
      end_of_databit = 1'b0;
      load_first_b   = 1'b0;
      scl_high       = 1'b1;
      SCL_SM         = 1'b1;
      SDA_OUT_SM     = 1'b1;
      SDA_DIR_SM     = 1'b0;
      SDA_SEL_SM     = 1'b1;
      read_phase     = 1'b0;
      start_phase    = 1'b0;
      transm_phase   = 1'b0;
      rep_write_seq  = 1'b0;
    end
  endcase
end

//========================================================================
//                        Combinatorial logic
//========================================================================

always @(*) begin
  LOAD_OUTPUT_SHIFTREG = load_first_b || (transm_phase && last_databit && end_of_databit && (transm_byte_counter != NUM_BYTES_TO_TRANSM));
end
always @(*) begin
  LATCH_FROM_CORE = start_phase;
end

assign LATCH_TO_CORE = read_phase && last_databit && end_of_databit;
assign SHIFT_IN = read_phase && scl_high;
assign SHIFT_OUT = transm_phase && end_of_databit;
assign RST_CORE_SIDE_REG = start_phase;

//========================================================================
//                          Sequential logic
//========================================================================

// Register for the repeated start
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    rep_start_reg   <= 1'b0;
  end else if (CLK_EN) begin
    if (rep_write_seq)
      rep_start_reg <= 1'b0;
    else if (START_SM)
      rep_start_reg <= REP_START;
  end
end

//========================================================================
//        Counter of the number of data bits transm or received
//========================================================================

// Comb logic
assign last_databit = (bit_counter == 3'd7);
assign incr_bit_counter = end_of_databit;
assign rst_bit_counter = end_of_databit && last_databit;

// Counter 
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    bit_counter   <= 3'd0;
  end else if (CLK_EN) begin
    if (rst_bit_counter)
      bit_counter <= 3'd0;
    else if (incr_bit_counter)
      bit_counter <= bit_counter + 1'b1;
  end
end

//========================================================================
//            Counter of the number of bytes read from slave
//========================================================================

// Comb logic
assign last_read_byte = (read_byte_counter == NUM_BYTES_TO_REC);
assign incr_read_byte_counter = rst_bit_counter && read_phase;
assign rst_read_byte_counter = end_of_ack && last_read_byte;

// Counter 
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    read_byte_counter   <= 5'd0;
  end else if (CLK_EN) begin
    if (rst_read_byte_counter)
      read_byte_counter <= 5'd0;
    else if (incr_read_byte_counter)
      read_byte_counter <= read_byte_counter + 1'b1;
  end
end

//========================================================================
//              Counter of the number of bytes transm to slave
//========================================================================

// Comb logic
assign last_transm_byte = (transm_byte_counter == NUM_BYTES_TO_TRANSM + 1'b1);  // Taking into account the first byte containing the slave address
assign incr_transm_byte_counter = rst_bit_counter && transm_phase;
assign rst_transm_byte_counter = end_of_ack && last_transm_byte;

// Counter 
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N) begin
    transm_byte_counter   <= 4'd0;
  end else if (CLK_EN) begin
    if (rst_transm_byte_counter)
      transm_byte_counter <= 4'd0;
    else if (rep_write_seq)
      transm_byte_counter <= 4'd1;
    else if (incr_transm_byte_counter)
      transm_byte_counter <= transm_byte_counter + 1'b1;
  end
end

endmodule
