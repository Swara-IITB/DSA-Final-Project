# =====================================================
# COMPILER AND FLAGS
# =====================================================
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall

# =====================================================
# PHASE 1
# =====================================================
PHASE1_SRC = \
    Phase-1/phase1.cpp \
    Phase-1/graph.cpp \
    Phase-1/loadGraph1.cpp \
    Phase-1/queryHandler.cpp \
    Phase-1/shortestPath.cpp \
	Phase-1/knn.cpp \

PHASE1_OUT = phase1

$(PHASE1_OUT): $(PHASE1_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(PHASE1_SRC)

# =====================================================
# PHASE 2
# =====================================================
PHASE2_SRC = \
    Phase-2/phase2.cpp \
    Phase-1/graph.cpp \
    Phase-1/loadGraph1.cpp \
    Phase-2/queryHandler2.cpp \
    Phase-2/Exact-kShortestPaths.cpp \
    Phase-2/Heuristic-kShortestPaths.cpp \
    Phase-2/approx_path.cpp \
    Phase-2/preprocess.cpp 

PHASE2_OUT = phase2

$(PHASE2_OUT): $(PHASE2_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(PHASE2_SRC)

# =====================================================
# PHASE 3
# =====================================================
PHASE3_SRC = \
    Phase-3/phase3.cpp \
    Phase-3/graph3.cpp \
    Phase-3/loadGraph3.cpp \
    Phase-3/queryHandler3.cpp \
    Phase-3/DeliveryScheduling.cpp \
    Phase-3/minimum_penalty_next_scheduler.cpp 

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
