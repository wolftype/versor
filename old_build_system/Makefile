GCC = 0 
GFX=1
PLATFORM=$(shell uname)

ifeq ($(PLATFORM),Linux)
	GCC = 1
endif

#PASS IN CLANG=path/to/clang++ if default is not working
CLANG = 0

#COMPILER VERSION  
ifeq ($(GCC),1) 
   CXX = g++ -std=c++0x 
else  
ifneq ($(CLANG),0) 
	CXX = $(CLANG)/bin/clang++
  #LDFLAGS = -L$(CLANG)/lib/  
else 
	CXX = clang++ 
endif 
	CXX += -arch x86_64
endif

ifeq ($(C14),1)
CXX += -std=c++14  $(USRFLAGS) 
else
CXX += -std=c++11  $(USRFLAGS)
endif

#OPTIMIZATION AND RECURSION
CXX += -O3 -ftemplate-depth-1200
#WARNINGS
CXX += -Wno-switch -Wno-int-to-pointer-cast -Wno-array-bounds -Wno-deprecated-declarations -Wno-c++1y-extensions

#LIB ARCHIVING
AR 	= ar crs 

#INCLUDES AND LINK PATHS
IPATH = -Ivsr/ -Iext/gfx/gfx/  -I/usr/include/ -I/usr/local/include 
LDFLAGS += -Lbuild/lib/ -Lext/glv/build/lib/ -lm  

#LINK TO GRAPHICS LIBRARIES
ifeq ($(GFX),1) 
IPATH += -Iext/glv/ -Iext/gfx/ -Iext/gfx/gfx -Iext/ 
ifeq ($(PLATFORM),Linux)
	LDFLAGS += -lGLV -lGLEW -lGL -lglut 
else
	LDFLAGS += -lglv -lGLEW -framework OpenGL -framework GLUT 
endif
endif
                           
LDFLAGS := $(LDFLAGS)  

SRC_DIR = src/
INCLUDE_DIR = vsr/
EXT_DIR = ext/
 
VPATH = $(SRC_DIR):\
		$(INCLUDE_DIR):\
		$(EXT_DIR)gl2ps 

EXEC = scratch/%.cpp examples/%.cpp

#LIBRARY SPACES
OBJ = vsr_cga3D_op.o vsr_cga3D_funcs.o vsr_cga3D_frame.o vsr_cga3D_cubicLattice.o  

#ADD GRAPHICS FUNCTTONALITY INTO LIBRARY
ifeq ($(GFX),1) 
OBJ += gl2ps.o vsr_cga3D_draw.o vsr_cga3D_render.o vsr_cga3D_xf.o 
endif

OBJ_DIR = build/obj/
LIB_DIR = build/lib/
BIN_DIR = build/bin/

LIB_NAME = vsr
LIB_FILE = libvsr.a

LDFLAGS += -l$(LIB_NAME)  

$(OBJ_DIR)%.o: %.cpp 
	@echo Building libvsr: compiling $@ using $< 
	$(CXX) $(IPATH) -c $< -o $@ 

$(OBJ_DIR)%.o: %.c
	@echo Building libvsr: compiling $@ using $< 
	$(CXX) $(IPATH) -c $< -o $@ 


$(LIB_NAME): dir $(addprefix $(OBJ_DIR),$(OBJ)) 
	@echo archiving $@
	$(AR) $(LIB_DIR)$(LIB_FILE) $(addprefix $(OBJ_DIR), $(OBJ))  

FORCE:

.PRECIOUS: $(EXEC)  
 
glv: 
	@echo "glv"
	@echo "building external GUI library GLV . . . if there are errors, make sure you have entered:\n\n"
	@echo "git submodule init"
	@echo "git submodule update\n\n"
	$(MAKE) --no-print-directory -C ext/glv
	#install DESTDIR=../../$(BUILD_DIR)    

$(EXEC): dir glv vsr FORCE 
	@echo Building $@
	$(CXX) -o $(BIN_DIR)$(*F) $@ $(IPATH) $(LDFLAGS)
	@cd $(BIN_DIR) && ./$(*F)

run:
	./$(BIN_DIR)$(NAME) 

deploy:
	scp -P $(PORT) $(BIN_DIR)$(NAME) $(HOST):/tmp 
	ssh -t -p $(PORT) $(HOST) /tmp/$(NAME) 

dir:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(BIN_DIR)  

clean:
	@rm -r $(OBJ_DIR)
	@rm -r $(LIB_DIR)
