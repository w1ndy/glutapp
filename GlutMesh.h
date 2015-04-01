#ifndef __GLUTMESH_H__
#define __GLUTMESH_H__

#include "BaseType.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

#define _DEBUG

template<typename T>
struct vertex_with_normal_t {
    T x, y, z;
    T nx, ny, nz;

    vertex_with_normal_t()
        : x(0), y(0), z(0), nx(0), ny(0), nz(0) {}

    vertex_with_normal_t(T _x, T _y, T _z)
        : x(_x), y(_y), z(_z), nx(0), ny(0), nz(0) {}

    void add_normal(Vector<T> const &E1, Vector<T> const &E2)
    {
        Vector<T> N = E1 * E2;
        //N *= N.length() / (E1.length() * E2.length());
        nx += N.x;
        ny += N.y;
        nz += N.z;
        //std::cout << "normal added for " << x << "," << y << "," << z << " is " << N.x << "," << N.y << "," << N.z << std::endl;
    }

    void normalize()
    {
        //std::cout << "normal before normalize for " << x << "," << y << "," << z << " is " << nx << "," << ny << "," << nz << std::endl;
        T len = sqrt(nx * nx + ny * ny + nz * nz);
        if(abs(len) < floating_point_type_properties<T>::epsilon)
            return ;
        nx /= len, ny /= len, nz /= len;
        //std::cout << "normal after normalize for " << x << "," << y << "," << z << " is " << nx << "," << ny << "," << nz << std::endl;
    }

    static constexpr size_t size = sizeof(vertex_with_normal_t);
};

enum mesh_type_t
{
    MESH_TYPE_TRIANGLES     = GL_TRIANGLES,

    MESH_TYPE_NOT_LOADED    = 0xffffffff
};

template<typename T, typename I>
class GlutMeshBase
{
public:
    typedef T vertex_elem_t;
    typedef I index_elem_t;

    typedef vertex_with_normal_t<vertex_elem_t> vertex_t;
    typedef std::vector<vertex_t> vertex_list_t;
    typedef std::vector<index_elem_t> index_list_t;

    static constexpr size_t vertex_elem_type_size = sizeof(vertex_elem_t);
    static constexpr size_t index_elem_type_size  = sizeof(index_elem_t);

public:
    GlutMeshBase();
    ~GlutMeshBase();

    bool fromObjFile(std::string const &fname);

    vertex_list_t &getVertexList()
    {
        assert(isLoaded());
        return _mesh->raw_mesh.vertex_array;
    }

    index_list_t &getIndexList()
    {
        assert(isLoaded());
        return _mesh->raw_mesh.index_array;
    }

    bool isLoaded() const
    {
        return _mesh != NULL;
    }

    mesh_type_t getType() const
    {
        return (isLoaded() ? _mesh->mesh_type : MESH_TYPE_NOT_LOADED);
    }

    bool rebuffer();
    void draw();
    void drawNormals();
    void free();

private:
    struct mesh_t {
        struct {
            vertex_list_t vertex_list;
            index_list_t index_list;
        } raw_mesh;

        struct {
            GLuint vbuf, ibuf;
        } gl_buffer;

        mesh_type_t mesh_type;
    };

    mesh_t *_mesh;

private:
    bool _make_buffer(mesh_type_t mesh_type, vertex_list_t &vertex_list,
        index_list_t &index_list);
};

typedef GlutMeshBase<float, unsigned short> GlutMesh;
typedef GlutMeshBase<double, unsigned short> GlutHighPrecisionMesh;

template<typename T, typename I>
GlutMeshBase<T, I>::GlutMeshBase()
{
    _mesh = NULL;
}

template<typename T, typename I>
GlutMeshBase<T, I>::~GlutMeshBase()
{
    if(isLoaded())
        free();
}

template<typename T, typename I>
void GlutMeshBase<T, I>::free()
{
    glDeleteBuffers(1, &(_mesh->gl_buffer.vbuf));
    glDeleteBuffers(1, &(_mesh->gl_buffer.ibuf));
    delete _mesh;
    _mesh = NULL;
}

template<typename T, typename I>
bool GlutMeshBase<T, I>::rebuffer()
{
    assert(isLoaded());
    vertex_list_t vertex_list = _mesh->raw_mesh.vertex_list;
    index_list_t index_list = _mesh->raw_mesh.index_list;
    return _make_buffer(_mesh->mesh_type, vertex_list, index_list);
}

template<typename T, typename I>
bool GlutMeshBase<T, I>::fromObjFile(std::string const &fname)
{
    std::ifstream fin(fname);
    if(!fin.is_open()) {
        std::cerr << "warning: couldn't open " << fname << std::endl;
        return false;
    }

    std::string s;
    vertex_t v;
    I a, b, c;

    vertex_list_t v_arr;
    index_list_t idx_arr;

    while(fin >> s) {
        if(s[0] == 'v') {
            fin >> v.x >> v.y >> v.z;
            v_arr.push_back(v);
        } else if(s[0] == 'f') {
            fin >> a >> b >> c;
            a--; b--; c--;
            idx_arr.push_back(a);
            idx_arr.push_back(b);
            idx_arr.push_back(c);
        } else if(s[0] == '#') {
            std::getline(fin, s);
        } else {
            std::cerr << "warning: unrecognized identifier " << s << std::endl;
        }
    }

    std::cout << "read " << v_arr.size() << " vertices and "
         << idx_arr.size() / 3 << " faces" << std::endl;

    return _make_buffer(MESH_TYPE_TRIANGLES, v_arr, idx_arr);
}

template<typename T, typename I>
void GlutMeshBase<T, I>::draw()
{
    assert(isLoaded());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, _mesh->gl_buffer.vbuf);
    glVertexPointer(3, primitive_type_to_gl_enum<vertex_elem_t>::value,
        vertex_t::size, 0);
    glNormalPointer(primitive_type_to_gl_enum<vertex_elem_t>::value,
        vertex_t::size, (void*)(3 * vertex_elem_type_size));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->gl_buffer.ibuf);
    glDrawElements(_mesh->mesh_type, _mesh->raw_mesh.index_list.size(),
        primitive_type_to_gl_enum<index_elem_t>::value, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

template<typename T, typename I>
void GlutMeshBase<T, I>::drawNormals()
{
    assert(isLoaded());

    glLineWidth(2.0f);
    for(auto &v : _mesh->raw_mesh.vertex_list) {
        glBegin(GL_LINES);
        glVertex3f(v.x, v.y, v.z);
        glVertex3f(v.x + v.nx, v.y + v.ny, v.z + v.nz);
        glEnd();
    }
}

template<typename T, typename I>
bool GlutMeshBase<T, I>::_make_buffer(
    mesh_type_t type, vertex_list_t &vertex_list, index_list_t &index_list)
{
    Vector<vertex_elem_t> dp, dx, dy, N;
    vertex_t v;

    if(vertex_list.size() > std::numeric_limits<index_elem_t>::max()) {
        std::cerr << "error: index possibly out of range." << std::endl;
        return false;
    }

    for(size_t i = 0; i < index_list.size(); i += 3) {
        dp.set(vertex_list[index_list[i]].x,
               vertex_list[index_list[i]].y,
               vertex_list[index_list[i]].z);
        dx.set(vertex_list[index_list[i + 1]].x,
               vertex_list[index_list[i + 1]].y,
               vertex_list[index_list[i + 1]].z);
        dy.set(vertex_list[index_list[i + 2]].x,
               vertex_list[index_list[i + 2]].y,
               vertex_list[index_list[i + 2]].z);

        dx -= dp;
        dy -= dp;

        for(size_t j = i; j < i + 3; j++)
            vertex_list[index_list[j]].add_normal(dx, dy);
    }

    for(size_t i = 0; i < vertex_list.size(); i++)
        vertex_list[i].normalize();

    if(isLoaded())
        free();

    _mesh = new mesh_t;

    if(!_mesh) {
        std::cerr << "Out of memory!" << std::endl;
        return false;
    }

    _mesh->raw_mesh.vertex_list = vertex_list;
    _mesh->raw_mesh.index_list = index_list;

    glGenBuffers(1, &(_mesh->gl_buffer.vbuf));
    glGenBuffers(1, &(_mesh->gl_buffer.ibuf));

    _mesh->mesh_type = type;

    glBindBuffer(GL_ARRAY_BUFFER, _mesh->gl_buffer.vbuf);
    glBufferData(GL_ARRAY_BUFFER,
        vertex_list.size() * vertex_t::size,
        vertex_list.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->gl_buffer.ibuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        index_list.size() * index_elem_type_size,
        index_list.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

#endif // __GLUTMESH_H__
