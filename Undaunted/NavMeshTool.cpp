#include "NavmeshTool.h"
#include "SKSELink.h"
#include <Undaunted\SpawnUtils.h>
namespace Undaunted
{
	std::string floattostring(float value)
	{
		char hex[9];
		sprintf(hex, "%08X", *(unsigned long int*) & value);
		//	_MESSAGE("float value: %s", hex);
		std::string output = "";
		output += hex[6];
		output += hex[7];
		output += ' ';
		output += hex[4];
		output += hex[5];
		output += ' ';
		output += hex[2];
		output += hex[3];
		output += ' ';
		output += hex[0];
		output += hex[1];
		return output;
	}

	std::string uint32tostring(UInt32 value)
	{
		char hex[9];
		sprintf(hex, "%04X", *(unsigned long int*) & value);
		std::string output = "";
		output += hex[2];
		output += hex[3];
		output += ' ';
		output += hex[0];
		output += hex[1];
		return output;
	}

	void AddVertex(Vert vertex)
	{
		_MESSAGE("verts := NewArrayElement(nvnm, 'Vertices');seev(nvnm, 'Vertices\\[%i]', '%s %s %s');", vertex.index, floattostring(vertex.x), floattostring(vertex.y), floattostring(vertex.z));
	}

	void AddTriangle(Triangle tri)
	{
		//Not bothering with cover right now, which is the last 4 values.
		_MESSAGE("NewArrayElement(nvnm, 'Triangles');seev(nvnm, 'Triangles\\[%i]', '%s %s %s %s %s %s 00 00 00 00');",
			tri.index, 
			uint32tostring(tri.vert1), 
			uint32tostring(tri.vert2),
			uint32tostring(tri.vert3),
			uint32tostring(tri.edge1),
			uint32tostring(tri.edge2),
			uint32tostring(tri.edge3));
	}

	// Remember to edit the navmesh slightly in the Creation Kit. Otherwise it seems to lead to an unending memory leak :D
	// I believe this is due to us not setting up the NavMeshGrid in the code, howeer making any change in the CK and saving it generates it.
	// Doing a balance for optimisation seems to work pretty well.
	TileMap currentMap;
	int QuadSize = 128;
	int corriderHeight = 256;
	
	void InitNavmesh()
	{

	}

	void ExportNavmesh()
	{

		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Navmesh.pas");
		_MESSAGE("unit userscript;uses SkyrimUtils;uses mteFunctions;");
		_MESSAGE("function NewArrayElement(rec: IInterface; path: String): IInterface; var a: IInterface; begin a := ElementByPath(rec, path); if Assigned(a) then begin Result := ElementAssign(a, HighInteger, nil, false); end else begin a := Add(rec, path, true);Result := ElementByIndex(a, 0);end;end;");
		_MESSAGE("function Process(e: IInterface): integer; var cell: IInterface; navm: IInterface; nvnm: IInterface; verts: IInterface; begin Result := 0;  if not (Signature(e) = 'CELL') then begin Exit; end; AddMessage('Processing: ' + FullPath(e));");
		_MESSAGE("navm := Add(e,'NAVM',true);nvnm := Add(navm,'NVNM',true);seev(nvnm, 'Version', 12);seev(nvnm, 'Parent Cell', HexFormID(e));seev(nvnm, 'NavMeshGrid', '00');");


		int VertCount = 0;
		int TriCount = 0;

		VertList createdVerts = VertList();
		TriangleList createdTriangles = TriangleList();
		
		for (int i = 0; i < currentMap.map.length; i++)
		{
			Vert Vert1 = Vert(0, (currentMap.map.data[i].x * (QuadSize * 2)) - QuadSize, (currentMap.map.data[i].y * (QuadSize * 2)) - QuadSize, (currentMap.map.data[i].z));
			UInt32 Vert1Index = -1;
			Vert1Index = createdVerts.Find(Vert1);
			if (Vert1Index == -1)
			{
				Vert1.index = VertCount++;
				AddVertex(Vert1);
				Vert1Index = Vert1.index;
				createdVerts.AddItem(Vert1);
			}

			Vert Vert2 = Vert(0, (currentMap.map.data[i].x * (QuadSize * 2)) + QuadSize, (currentMap.map.data[i].y * (QuadSize * 2)) - QuadSize, (currentMap.map.data[i].z));
			UInt32 Vert2Index = -1;
			Vert2Index = createdVerts.Find(Vert2);
			if (Vert2Index == -1)
			{
				Vert2.index = VertCount++;
				AddVertex(Vert2);
				Vert2Index = Vert2.index;
				createdVerts.AddItem(Vert2);
			}

			Vert Vert3 = Vert(0, (currentMap.map.data[i].x * (QuadSize * 2)) + QuadSize, (currentMap.map.data[i].y * (QuadSize * 2)) + QuadSize, (currentMap.map.data[i].z));
			UInt32 Vert3Index = -1;
			Vert3Index = createdVerts.Find(Vert3);
			if (Vert3Index == -1)
			{
				Vert3.index = VertCount++;
				AddVertex(Vert3);
				Vert3Index = Vert3.index;
				createdVerts.AddItem(Vert3);
			}

			Vert Vert4 = Vert(0, (currentMap.map.data[i].x * (QuadSize * 2)) - QuadSize, (currentMap.map.data[i].y * (QuadSize * 2)) + QuadSize, (currentMap.map.data[i].z));
			UInt32 Vert4Index = -1;
			Vert4Index = createdVerts.Find(Vert4);
			if (Vert4Index == -1)
			{
				Vert4.index = VertCount++;
				AddVertex(Vert4);
				Vert4Index = Vert4.index;
				createdVerts.AddItem(Vert4);
			}

			Triangle tri1 = Triangle(TriCount++, Vert1Index, Vert2Index, Vert3Index, -1, -1, -1);
			createdTriangles.AddItem(tri1);

			Triangle tri2 = Triangle(TriCount++, Vert1Index, Vert4Index, Vert3Index, -1, -1, -1);
			createdTriangles.AddItem(tri2);

		}
		//Squish the triangles which are layer on top of each other
		for (int i = 0; i < createdTriangles.length; i++)
		{

		}

		//Join the triangles
		for (int i = 0; i < createdTriangles.length; i++)
		{
			createdTriangles.data[i].edge1 = createdTriangles.FindNeighbours(createdTriangles.data[i], 1, createdVerts);
			createdTriangles.data[i].edge2 = createdTriangles.FindNeighbours(createdTriangles.data[i], 2, createdVerts);
			createdTriangles.data[i].edge3 = createdTriangles.FindNeighbours(createdTriangles.data[i], 3, createdVerts);
		}

		//Save the triangles
		for (int i = 0; i < createdTriangles.length; i++)
		{
			AddTriangle(createdTriangles.data[i]);
		}

		_MESSAGE("end;end.");
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Undaunted.log");
	}

	void MarkTile(float x, float y, float z)
	{
		//(size / 2) * (QuadSize * 2))
		//(QuadSize * 2)
		x = x / (QuadSize * 2);
		y = y / (QuadSize * 2);
		z = z;

		int mapx = x; 
		int mapy = y;
		int mapz = z;


		if (currentMap.map.Find(mapx, mapy, z) == 0)
		{
			currentMap.map.AddItem(Tile(mapx, mapy, z, 1));

			//WorldCell wc = WorldCell();
			//wc.cell = Undaunted::GetPlayer()->parentCell;
			//wc.world = Undaunted::GetPlayer()->currentWorldSpace;
			//SpawnRefAtPosition(0x000B8A62, wc, NiPoint3(mapx, mapy, mapz));
		}
		_MESSAGE("PlayerPos %f, %f, %f", x,y,z);
		_MESSAGE("currentMapPos %i, %i, %i", mapx, mapy, mapz);

	}

	VertList* VertList::AddItem(Vert item)
	{
		VertList* currentlist = this;
		VertList newlist = VertList();
		newlist.length = currentlist->length + 1;
		newlist.data = new Vert[newlist.length];
		for (int i = 0; i < currentlist->length; i++)
		{
			newlist.data[i] = currentlist->data[i];
		}
		newlist.data[currentlist->length] = item;
		currentlist->data = newlist.data;
		currentlist->length = newlist.length;
		return currentlist;
	}

	UInt32 VertList::Find(Vert item)
	{
		VertList* currentlist = this;
		for (int i = 0; i < currentlist->length; i++)
		{
			float zdist = currentlist->data[i].z - item.z;
			zdist = zdist * zdist;//Square it to make it always positive

			if (currentlist->data[i].x == item.x &&
				currentlist->data[i].y == item.y &&
				zdist < (corriderHeight*corriderHeight))
			{
				return currentlist->data[i].index;
			}
		}
		return -1;
	}

	Vert VertList::FindIndex(UInt32 item)
	{
		VertList* currentlist = this;
		for (int i = 0; i < currentlist->length; i++)
		{
			if (currentlist->data[i].index == item)
			{
				return currentlist->data[i];
			}
		}
	}


	TriangleList* TriangleList::AddItem(Triangle item)
	{
		TriangleList* currentlist = this;
		TriangleList newlist = TriangleList();
		newlist.length = currentlist->length + 1;
		newlist.data = new Triangle[newlist.length];
		for (int i = 0; i < currentlist->length; i++)
		{
			newlist.data[i] = currentlist->data[i];
		}
		newlist.data[currentlist->length] = item;
		currentlist->data = newlist.data;
		currentlist->length = newlist.length;
		return currentlist;
	}

	/*
		Edge 1	uint16	Index within this list of the triangle that neighbors the first edge (vertex 0 to vertex 1)
		Edge 2	uint16	Index within this list of the triangle that neighbors the second edge (vertex 1 to vertex 2)
		Edge 3	uint16	Index within this list of the triangle that neighbors the third edge (vertex 2 to vertex 1)
	*/
	UInt32 TriangleList::FindNeighbours(Triangle item, int edge, VertList vlist)
	{
		TriangleList* currentlist = this;
		if (edge == 1 || edge == 2)
		{
			for (int i = 0; i < currentlist->length; i++)
			{
				//Don't join with yourself.
				if (currentlist->data[i].index != item.index)
				{
					Vert t1v1 = vlist.FindIndex(currentlist->data[i].vert1);
					Vert t1v2 = vlist.FindIndex(currentlist->data[i].vert2);
					Vert t1v3 = vlist.FindIndex(currentlist->data[i].vert3);

					Vert t2v1 = vlist.FindIndex(item.vert1);
					Vert t2v2 = vlist.FindIndex(item.vert2);
					Vert t2v3 = vlist.FindIndex(item.vert3);

					//South / West?
					if (t1v1.x == t2v2.x && t1v1.y == t2v2.y && 
						t1v2.x == t2v3.x && t1v2.y == t2v3.y )
					{
						return currentlist->data[i].index;
					}
				}
			}
		}
		if (edge == 3)
		{
			for (int i = 0; i < currentlist->length; i++)
			{
				Vert t1v1 = vlist.FindIndex(currentlist->data[i].vert1);
				Vert t1v2 = vlist.FindIndex(currentlist->data[i].vert2);
				Vert t1v3 = vlist.FindIndex(currentlist->data[i].vert3);

				Vert t2v1 = vlist.FindIndex(item.vert1);
				Vert t2v2 = vlist.FindIndex(item.vert2);
				Vert t2v3 = vlist.FindIndex(item.vert3);
				//Don't join with yourself.
				if (currentlist->data[i].index != item.index)
				{
					//Inner
					if (t1v1.x == t2v1.x && t2v1.y == t2v1.y &&
						t1v3.x == t2v3.x && t1v3.y == t2v3.y)
					{
						return currentlist->data[i].index;
					}
				}
			}
		}
		return -1;
	}

	TileList* TileList::AddItem(Tile item)
	{
		TileList* currentlist = this;
		TileList newlist = TileList();
		newlist.length = currentlist->length + 1;
		newlist.data = new Tile[newlist.length];
		for (int i = 0; i < currentlist->length; i++)
		{
			newlist.data[i] = currentlist->data[i];
		}
		newlist.data[currentlist->length] = item;
		currentlist->data = newlist.data;
		currentlist->length = newlist.length;
		return currentlist;
	}

	
	int TileList::Find(int x, int y, int z)
	{
		TileList* currentlist = this;
		for (int i = 0; i < currentlist->length; i++)
		{
			if (currentlist->data[i].x == x &&
				currentlist->data[i].y == y &&
				currentlist->data[i].z == z)
			{
				return currentlist->data[i].value;
			}
		}
		return 0;
	}
}