from VoxelLayer import VoxelLayer

def export_obj_by_layer(numColor, colors, voxelSize, bottomZ, voxelObjects):
    # open obj files and mtl files

    voxelLayer = VoxelLayer(colors, numColor, voxelSize, bottomZ)

    # update vertices and faces based on colors
    for j in range(len(colors)):
        for i in range(len(colors[0])):
            if colors[j][i] != 0:
                # number of verrtices from previous layers
                numExistingVertices = len(voxelObjects[colors[j][i]].vertices)
                voxelLayer.update_vertices_and_faces(i,j,numExistingVertices)

    return voxelLayer.facesByMaterial, voxelLayer.verticesByMaterial