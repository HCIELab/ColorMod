#
# import os
# import bpy
# import sys
# basedir =os.path.dirname(bpy.data.filepath)
# if basedir not in sys.path:
#     sys.path.append(basedir)

from VoxelObj import VoxelObj
#sys.path.append(r'C:\Users\xinwen\Documents\Blender\load_obj.py')
from load_obj import load_obj
#sys.path.append(r'C:\Users\xinwen\Documents\Blender\export_obj_by_layer.py')
from export_obj_by_layer import export_obj_by_layer
#sys.path.append(r'C:\Users\xinwen\Documents\Blender\create_voxel_obj.py')


def export_obj(voxelModel, numColor, blockSize, depth):
    files = {}
    mtl = {}
    numExistingVertices = {}
    voxelObjects = {}

    # create obj files and mtl files
    for i in range(numColor + 1):

        mtl[i + 1] = open("material%d.mtl" % (i + 1), "w+")
        mtl[i + 1].write("# Blender MTL File: 'None'" + "# Material Count: 1 newmtl NoneNs 0" + \
                         "Ka 0.000000 0.000000 0.000000Kd 0.8 0.8 0.8Ks 0.8 0.8 0.8d 1illum 2")
        mtl[i+1].close()
        files[i + 1] = open("material%d.obj" % (i + 1), "w+")

        numExistingVertices[i+1] = 0
        # material 1 - numColor: outside photochromix, material numColor: inside regular material
        files[i + 1].write("# OBJ file for material %d\n" % (i + 1))

        files[i + 1].write("mtllib material%d.mtl\n" % (i + 1))
        files[i + 1].write("o Model.00%d\n" % (i + 1))
        # write normal vectors
        files[i+1].write("vn -1.0000 0.0000 0.0000\n")
        files[i+1].write("vn 0.0000 0.0000 -1.0000\n")
        files[i+1].write("vn 1.0000 0.0000 0.0000\n")
        files[i+1].write("vn 0.0000 0.0000 1.0000\n")
        files[i+1].write("vn 0.0000 -1.0000 0.0000\n")
        files[i+1].write("vn 0.0000 1.0000 0.0000\n")
        # write material
        files[i+1].write("usemtl None\n")
        files[i+1].write("s off\n")
        files[i+1].close()
        #obj = create_voxel_obj(i+1)
        obj = VoxelObj(i+1)
        voxelObjects[i+1] = obj

    # update vertices and faces
    verticesOrganizedByZ = voxelModel.organize_vertices_by_z()
    for i in range(len(voxelModel.Z)-1):
        z1 = voxelModel.Z[i]
        z2 = voxelModel.Z[i + 1]
        validCoord, validFaces, validEdges = voxelModel.vertices_per_layer(z1, verticesOrganizedByZ[z1], z2,
                                                                           verticesOrganizedByZ[z2])
        blockContents = voxelModel.grid_per_layer(validCoord, validFaces, z1, z2, validEdges)

        # if i==len(voxelModel.Z)-2:
        #     blockColors = voxelModel.determine_top_color(blockContents, numColor, z1)
        # elif i==0:
        #     blockColors = voxelModel.determine_bottom_color(blockContents, numColor, z1)
        # else:
        #     z0 = voxelModel.Z[i-1]
        #     z3 = voxelModel.Z[i+2]
        #     validCoord, validFaces, validEdges = voxelModel.vertices_per_layer(z0, verticesOrganizedByZ[z0], z1,
        #                                                                        verticesOrganizedByZ[z1])
        #     blockContentsBottom = voxelModel.grid_per_layer(validCoord, validFaces, z0, z1, validEdges)
        #     validCoord, validFaces, validEdges = voxelModel.vertices_per_layer(z2, verticesOrganizedByZ[z2], z3,
        #                                                                        verticesOrganizedByZ[z3])
        #     blockContentsTop = voxelModel.grid_per_layer(validCoord, validFaces, z2, z3, validEdges)
        #     exposed = voxelModel.determine_exposed(blockContentsBottom, blockContents, blockContentsTop,z1)
        #
        #     #blockColors = voxelModel.determine_color_per_layer(blockContents, numColor, z1, exposed)
        if i == 0:
            z3 = voxelModel.Z[i + 2]
            blockContentsTop = voxelModel.grid_per_layer(validCoord, validFaces, z2, z3, validEdges)
            voxelModel.determine_exposed_bottom(blockContents, blockContentsTop)
        elif i == len(voxelModel.Z)-2:
            voxelModel.determine_exposed_top(blockContents)
        else:
            z0 = voxelModel.Z[i-1]
            z3 = voxelModel.Z[i+2]
            validCoord, validFaces, validEdges = voxelModel.vertices_per_layer(z0, verticesOrganizedByZ[z0], z1,
                                                                               verticesOrganizedByZ[z1])
            blockContentsBottom = voxelModel.grid_per_layer(validCoord, validFaces, z0, z1, validEdges)
            validCoord, validFaces, validEdges = voxelModel.vertices_per_layer(z2, verticesOrganizedByZ[z2], z3,
                                                                               verticesOrganizedByZ[z3])
            blockContentsTop = voxelModel.grid_per_layer(validCoord, validFaces, z2, z3, validEdges)
            voxelModel.determine_exposed(blockContentsBottom, blockContents, blockContentsTop,i)

        #blockColors = voxelModel.determine_color_per_layer(blockContents, numColor, z1, exposed)
    #voxelModel.determine_color_whole_model(numColor)
    voxelModel.determine_color_blocks(numColor, blockSize, depth)
    for i in range(len(voxelModel.Z) - 1):
        z1 = voxelModel.Z[i]
        z2 = voxelModel.Z[i + 1]
        blockColors = voxelModel.colors[i]
        layerFaces, layerVertices = export_obj_by_layer(numColor, blockColors, voxelModel.voxelSize, z1, voxelObjects)
        for material in range(1,numColor+2):
            obj = voxelObjects[material]
            obj.update(layerFaces[material], layerVertices[material])

    # write vertices and faces to file
    for i in range(numColor + 1):

        files[i + 1] = open("material%d.obj" % (i + 1), "a+")
        obj = voxelObjects[i+1]
        for (x,z,y) in obj.vertices:
            files[i+1].write("v %f %f %f\n" %(x,z,y))
        for ((v1, v2, v3, v4),vn) in obj.faces:
            files[i+1].write("f %d//%d %d//%d %d//%d %d//%d\n" %(v1, vn, v2, vn, v3, vn, v4, vn))


#filename = os.path.join(basedir , "turtle_simple.obj")
#filename =  "slab.obj"
#voxelizeModel = load_obj(filename)
#export_obj(voxelizeModel, 2, 4)

