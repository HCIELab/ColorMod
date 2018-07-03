from VoxelModel import VoxelModel
# do the loading of the obj file
def load_obj(filepath):
    V = []  # vertex
    T = []  # texcoords
    N = []  # normals
    F = []  # face indexies

    fh = open(filepath)
    for line in fh:
        if line[0] == '#': continue

        line = line.strip().split(' ')
        if line[0] == 'v':  # vertex
            V.append([float(i) for i in line[1:]])
            #V.append(map(float, line[1:]))
        elif line[0] == 'vt':  # texture
            T.append([float(i) for i in line[1:]])
            #T.append(map(float, line[1:]))
        elif line[0] == 'vn':  # normal vector
            N.append([float(i) for i in line[1:]])
            #N.append(map(float, line[1:]))
        elif line[0] == 'f':  # face
            face = line[1:]
            if len(face) != 4:
                print (line)
                # raise Exception('not a quad!')
                continue
            for i in range(0, len(face)):
                face[i] = face[i].split('/')
                # OBJ indexies are 1 based not 0 based hence the -1
                # convert index to integer
                for j in range(0, len(face[i])):
                    if face[i][j] != "":
                        face[i][j] = int(face[i][j]) - 1
            F.append(face)


    return VoxelModel(V, T, N, F)

# voxelModel1 = load_obj("cone_medium.obj")
# verticesOrganizedByZ = voxelModel1.organize_vertices_by_z()
# z1 = voxelModel1.Z[0]
# z2 = voxelModel1.Z[1]
# validCoord, validFaces, validEdges = voxelModel1.vertices_per_layer(z1, verticesOrganizedByZ[z1], z2, verticesOrganizedByZ[z2])
# blockContents = voxelModel1.grid_per_layer(validCoord, validFaces, z1, z2, validEdges)
# blockColors = voxelModel1.determine_color_per_layer(blockContents, 2, z1)
# print "blocks", blockContents
# print "colors", blockColors