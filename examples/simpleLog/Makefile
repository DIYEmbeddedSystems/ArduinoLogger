# Project specific configuration
PORT          = /dev/ttyUSB0
BOARD         = esp8266:esp8266:d1
LOG_DURATION  = 10

# Set a timestamp for all files created by this run of make
TSTAMP = $(shell date +"%Y%m%d-%H%M")


SKETCH_FILE   = $(shell ls -1 $(CURDIR)/*.ino | head -n1)
SKETCH_NAME   = $(shell basename $(CURDIR))
SKETCH_BASE   = $(CURDIR)
MONITOR_SPEED = $(shell egrep Serial.begin $(SKETCH_FILE) | perl -pE 's/\D+//g' | head -n1)
BUILD_DIR     = $(CURDIR)/build

DATA_DIR     = $(CURDIR)/data
MKSPIFFS    = $(shell find ~/.ardui* -type f -name mkspiffs | head -n1)
ESPTOOL     = $(shell find ~/.ardui* -type f -name esptool.py | head -n1)

SPIFFS_IMG  = $(BUILD_DIR)/$(SKETCH_NAME).spiffs
# Should be 1028096,  2076672, or  3125248 (1MB, 2MB, or 3MB)
SPIFFS_SIZE = 1028096
# Should be 0x300000 for 1MB, 0x200000 for 2MB, or 0x100000 for 3MB
SPIFFS_ADDR = 0x300000

LOG_DIR     = $(CURDIR)/logs

# Create subdirectories, if necssary
prepare:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/cache
	mkdir -p $(BUILD_DIR)/build
	mkdir -p $(LOG_DIR)


# Output current configuration
dump_config:
	@echo
	@echo "BOARD         : $(BOARD)"
	@echo "PORT          : $(PORT)"
	@echo "MONITOR SPEED : $(MONITOR_SPEED)"
	@echo "SKETCH NAME   : $(SKETCH_NAME)"
	@echo "SKETCH FILE   : $(SKETCH_FILE)"
	@echo "Found ESPTOOL : $(ESPTOOL)"
	@echo "Found MKSPIFFS: $(MKSPIFFS)"
	@echo


# Compile current sketch
verify: prepare
	@echo
	@echo Buildind sketch $(SKETCH_NAME)
	@echo
	arduino-cli compile --fqbn $(BOARD) --build-cache-path $(BUILD_DIR)/cache --build-path $(BUILD_DIR)/build --log-file $(LOG_DIR)/Compile.$(TSTAMP).log --verbose $(CURDIR)

compile: verify

# Upload current sketch to target
upload_sketch: 
	@echo
	@echo Uploading sketch to $(BOARD) at port $(PORT)
	@echo
	arduino-cli upload --fqbn $(BOARD) --port $(PORT) --verify --log-file $(LOG_DIR)/Upload.$(TSTAMP).log --verbose $(CURDIR)

# Create SPIFFS image from files in the data/ subdirectory
spiffs_image: prepare
	@echo
	@echo Building SPIFFS image
	@echo
	$(MKSPIFFS) -c $(DATA_DIR) --page 256 --block 8192 -s $(SPIFFS_SIZE) $(SPIFFS_IMG) |tee -i $(LOG_DIR)/MkSPIFFS.$(TSTAMP).log

# Upload SPIFFS image
upload_spiffs: spiffs_image
	@echo
	@echo uploading SPIFFS image
	@echo
	$(ESPTOOL) --chip auto --baud 460800 --port $(PORT) write_flash $(SPIFFS_ADDR) $(SPIFFS_IMG) |tee -i $(LOG_DIR)/LoadSPIFFS.$(TSTAMP).log


clean:
	rm -rf *.bin *.elf build logs

# Log execution for a certain time
log:
	@echo
	@echo Extract an execution log for $(LOG_DURATION)seconds
	@echo
	# Set port to corresponding baudrate
	stty -F $(PORT) $(MONITOR_SPEED)
	#timeout $(LOG_DURATION)s cat $(PORT) |tee -i $(SKETCH_NAME).$(shell date +"%Y%m%d-%H%M").log
	timeout $(LOG_DURATION) sh -c "(stty raw; cat > $(LOG_DIR)/$(SKETCH_NAME).$(TSTAMP).log) < $(PORT)" || echo "timed out"
	cat $(LOG_DIR)/$(SKETCH_NAME).$(TSTAMP).log



.PHONY: log monitor clean upload_spiffs upload_sketch spiffs_image verify compile
