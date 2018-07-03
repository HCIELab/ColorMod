from load_obj import load_obj

filepath = "cone_simple.obj"
def extract_voxel(filepath):
    V, T, N, F = load_obj(filepath)
    sortedLayers , verticesByZ = organize_by_z(V)





    #voxels = [[] for i in range(len(sortedLayers))]




def organize_by_z(V):
    verticesOrganizedByZ = {z:set([]) for z in sortedLayers}
    for [x, z, y] in V:
        verticesOrganizedByZ[z].add((x, y))
    return sortedLayers, verticesOrganizedByZ

def vertices_per_layer(layeriZ, verticesOnLayeri, layerjZ, veticesOnLayerj, coordToIndex, verticestoFace):
    validCoord = set([])
    validFaces = set([])
    # keep only vertices that appear in both layers
    validCoordPreliminary = list(filter(lambda (x,y):(x,y) in veticesOnLayerj, verticesOnLayeri))
    sorted_horizontal_validCoordPreliminary = sorted(validCoordPreliminary, key=lambda x:(x[0], x[1]))
    sorted_vertical_validCoordPreliminary = sorted(validCoordPreliminary, key=lambda x:(x[1], x[0]))

    # filter vertices connected by a horizontal face between two layers
    for i in range(len(sorted_horizontal_validCoordPreliminary)-1):
        # if two vertices on the same x plane, check if they are connected by a face
        if sorted_horizontal_validCoordPreliminary[i][0] == sorted_horizontal_validCoordPreliminary[i+1][0]:
            (x1, y1) = sorted_horizontal_validCoordPreliminary[i]
            (x2, y2) = sorted_horizontal_validCoordPreliminary[i+1]
            index1 = coordToIndex[(x1, layeriZ, y1)]
            index2 = coordToIndex[(x1, layerjZ, y1)]
            index3 = coordToIndex[(x2, layeriZ, y2)]
            index4 = coordToIndex[(x2, layerjZ, y2)]
            faceVertices = [index1,index2, index3, index4]
            faceVertices.sort()
            if tuple(faceVertices) in verticestoFace:
                validCoord.add((x1, y1))
                validCoord.add((x2, y2))
                validFaces.add(tuple(faceVertices))

    # filter vertices connected by a vertical face between two layers
    for i in range(len(sorted_vertical_validCoordPreliminary)-1):
        # if two vertices on the same x plane, check if they are connected by a face
        if sorted_vertical_validCoordPreliminary[i][1] == sorted_vertical_validCoordPreliminary[i+1][1]:
            (x1, y1) = sorted_vertical_validCoordPreliminary[i]
            (x2, y2) = sorted_vertical_validCoordPreliminary[i+1]
            index1 = coordToIndex[(x1, layeriZ, y1)]
            index2 = coordToIndex[(x1, layerjZ, y1)]
            index3 = coordToIndex[(x2, layeriZ, y2)]
            index4 = coordToIndex[(x2, layerjZ, y2)]
            faceVertices = [index1,index2, index3, index4]
            faceVertices.sort()
            if tuple(faceVertices) in verticestoFace:
                validCoord.add((x1, y1))
                validCoord.add((x2, y2))
                validFaces.add(tuple(faceVertices))

    print "validCoord", validCoord
    print "validFaces", validFaces
    return validCoord, validFaces

def voxel_per_layer(layeriZ, layerjZ, validCoord, validFaces):
    voxel_centers = [] #(x, y, z)



V, T, N, F = load_obj(filepath)
sortedLayers, verticesOrganizedByZ = organize_by_z(V)
coordToIndex = {}
for i in range(len(V)):
    coordToIndex[tuple(V[i])] = i
verticesToFace_temp = []
for face in F:
    vertices = [face[0][0], face[1][0], face[2][0], face[3][0]]
    vertices.sort()
    verticesToFace_temp.append(tuple(vertices))
verticesToFace = set(verticesToFace_temp)
z_i = sortedLayers[2]
z_j = sortedLayers[3]
print z_i, z_j
vertices_per_layer(z_i, verticesOrganizedByZ[z_i], z_j, verticesOrganizedByZ[z_j], coordToIndex, verticesToFace)