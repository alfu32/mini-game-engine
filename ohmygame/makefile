CC = gcc
CFLAGS = -Wall -shared -fPIC
LIBNAME = ohmygame

INCLUDE_DIR = include
SRC_DIR = .

HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ_DIR = build/object
LIB_DIR = build
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(LIB_DIR)/lib$(LIBNAME).so

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCLUDE_DIR)

$(LIB_DIR)/lib$(LIBNAME).so: $(OBJ)
	@mkdir -p $(LIB_DIR)
	$(CC) $(CFLAGS) -o $@ $^

install: $(LIB_DIR)/lib$(LIBNAME).so
	@cp $< /usr/lib/
	@mkdir -p /usr/include/$(LIBNAME)
	@cp -r $(INCLUDE_DIR)/* /usr/include/$(LIBNAME)/
	@mkdir -p /usr/lib/pkgconfig
	@cp $(LIBNAME).pc /usr/lib/pkgconfig/
uninstall:
	@rm -rf /usr/lib/$(LIBNAME).so
	@rm -rf /usr/include/$(LIBNAME)
	@rm -f /usr/lib/pkgconfig/$(LIBNAME).pc

clean:
	@rm -rf $(OBJ_DIR) $(LIB_DIR)

