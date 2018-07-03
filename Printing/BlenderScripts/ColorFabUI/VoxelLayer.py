# # import os
# bl_info = {
#     "name": "Voxel Layer",
#     "category": "Object",
# }
#
# import bpy
# import sys
#
# basedir =os.path.dirname(bpy.data.filepath)
# if basedir not in sys.path:
#     sys.path.append(basedir)

class VoxelLayer():
    def __init__(self, colors, numColors, voxelSize, bottomZ):
        self.colors = colors
        self.numColors = numColors
        self.voxelSize = voxelSize
        self.bottomZ = bottomZ
        self.verticesByMaterial = {}
        self.facesByMaterial = {}
        for i in range(1,numColors+2):
            self.verticesByMaterial[i] = []
            self.facesByMaterial[i] = set([])


    def update_vertices_and_faces(self, i, j, numExistingVertices):
        material = self.colors[j][i]
        # number of vertices = vertices generated from this layer so far + vertices from previous layers
        n = len(self.verticesByMaterial[material])+numExistingVertices
        vertices = []
        offset = [(-1, -1, 1), (-1, 1, 1), (-1, -1, -1), (-1, 1, -1), (1, -1, 1), (1, 1, 1), (1, -1, -1), (1, 1, -1)]
        center = ((i + 0.5) * self.voxelSize, (self.bottomZ + self.voxelSize * 0.5), (j + 0.5) * self.voxelSize)
        for (x, y, z) in offset:
            vertices.append((x * 0.5 * self.voxelSize + center[0], z * 0.5 * self.voxelSize + center[1], y * 0.5 * self.voxelSize + center[2]))
        # add vertices from this cube
        self.verticesByMaterial[material] += vertices
        # add faces from this cube
        self.facesByMaterial[material].add(((n + 1, n + 2, n + 4, n + 3), 1))
        self.facesByMaterial[material].add(((n + 3, n + 4, n + 8, n + 7), 2))
        self.facesByMaterial[material].add(((n + 7, n + 8, n + 6, n + 5), 3))
        self.facesByMaterial[material].add(((n + 5, n + 6, n + 2, n + 1), 4))
        self.facesByMaterial[material].add(((n + 3, n + 7, n + 5, n + 1), 5))
        self.facesByMaterial[material].add(((n + 8, n + 4, n + 2, n + 6), 6))

# def register():
#     bpy.utils.register_class(VoxelLayer)
# def unregister():
#     bpy.utils.unregister_class(VoxelLayer)
#
# if __name__ == "__main__":
#     register()
