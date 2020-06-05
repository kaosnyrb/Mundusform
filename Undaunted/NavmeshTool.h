namespace Undaunted
{
#ifndef NavMeshTool
#define NavMeshTool
	class Tile {
	public:
		int x;
		int y;
		int z;
		int value;
		Tile(int _x, int _y, int _z, int _value)
		{
			x = _x;
			y = _y;
			z = _z;
			value = _value;
		}
		Tile(){}
	};
	class TileList {
	public:
		Tile* data;
		int length;
		TileList* AddItem(Tile item);
		int Find(int x, int y, int z);
	};

	class TileMap {
	public:
		int size = 4096;
		int height = 256;
		TileList map;
	};

	class Vert {
	public:
		UInt32 index;
		float x, y, z;
		Vert(UInt32 _index, float _x, float _y, float _z)
		{
			index = _index;
			x = _x;
			y = _y;
			z = _z;
		}
		Vert() {

		}
	};

	class VertList {
	public:
		Vert* data;
		int length;
		VertList* AddItem(Vert item);
		UInt32 Find(Vert item);
		Vert FindIndex(UInt32 item);

	};


	class Triangle {
	public:
		UInt32 index;
		UInt32 vert1, vert2, vert3; //Index of a vertex
		UInt32 edge1, edge2, edge3; //Index of a different triangle.
		Triangle(UInt32 _index, UInt32 _vert1, UInt32 _vert2, UInt32 _vert3, UInt32 _edge1, UInt32 _edge2, UInt32 _edge3)
		{
			index = _index;
			vert1 = _vert1;
			vert2 = _vert2;
			vert3 = _vert3;
			edge1 = _edge1;
			edge2 = _edge2;
			edge3 = _edge3;
		}
		Triangle(){}
	};

	class TriangleList {
	public:
		Triangle* data;
		int length;
		TriangleList* AddItem(Triangle item);
		UInt32 FindNeighbours(Triangle item, int edge, VertList vlist);
	};

	void InitNavmesh();
	void ExportNavmesh();
	void MarkTile(float x, float y, float z);
#endif
}