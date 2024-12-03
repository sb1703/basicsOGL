#include "config.h"

class TriangleMesh2 {
    public:
        TriangleMesh2();
        void draw2();
        ~TriangleMesh2();
    private:
        unsigned int EBO, VAO, vertex_count;
        std::vector<unsigned int> VBOs;
};