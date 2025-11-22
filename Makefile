# =====================================================
# COMPILER AND FLAGS
# =====================================================
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall

# =====================================================
# PHASE 1
# =====================================================
PHASE1_SRC = \
    Phase1/phase1.cpp \
    Phase1/graph.cpp \
    Phase1/loadGraph1.cpp \
    Phase1/queryHandler.cpp \
    Phase1/shortestPath.cpp \
	Phase1/knn.cpp \

PHASE1_OUT = phase1

$(PHASE1_OUT): $(PHASE1_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(PHASE1_SRC)

# =====================================================
# PHASE 2
# =====================================================
PHASE2_SRC = \
    Phase2/phase2.cpp \
    Phase1/graph.cpp \
    Phase1/loadGraph1.cpp \
    Phase2/queryHandler2.cpp \
    Phase2/Exact-kShortestPaths.cpp \
    Phase2/Heuristic-kShortestPaths.cpp \
    Phase2/approx_path.cpp \
    Phase2/preprocess.cpp 

PHASE2_OUT = phase2

$(PHASE2_OUT): $(PHASE2_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(PHASE2_SRC)

# =====================================================
# PHASE 3
# =====================================================
PHASE3_SRC = \
    Phase3/phase3.cpp \
    Phase3/graph3.cpp \
    Phase3/loadGraph3.cpp \
    Phase3/queryHandler3.cpp \
    Phase3/DeliveryScheduling.cpp \
    Phase3/minimum_penalty_next_scheduler.cpp 

PHASE3_OUT = phase3

$(PHASE3_OUT): $(PHASE3_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(PHASE3_SRC)

# =====================================================
# PHONY TARGETS
# =====================================================
.PHONY: all clean

all: $(PHASE1_OUT) $(PHASE2_OUT) $(PHASE3_OUT)

clean:
	rm -f phase1 phase2 phase3
