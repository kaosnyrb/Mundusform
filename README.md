# Mundusform
Skyrim SE tool for worldbuilding

The navmeshing/Dungeon tools seemed too useful to lump in with Undaunted so they've been split out here.


# Tools

# Player location based navmesh generation 

Tracks the players movement through a cell and places joined quads under their feet.

This can then be exported to a sse edit script which will create the navmesh on the targeted cell.

Once applied the navmeshes are standard nav meshes, with no dependency on mundusform.

This should help rough out a navmesh that can be refined if necessary.



# Wip

# block based dungeon generation 

Similar to games like diablo and warframe, defining tiles in a json format and procedural generating a complete dungeon with working navmesh.
Built to support Undaunted rifts but built to be useful for quickly laying out dungeons for any author.

Output will be a sseedit script so the output will be completely editable via standard tools.

