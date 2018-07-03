# bl_info = {
#     "name": "Voxel Model",
#     "category": "Object",
# }
#import bpy
# import os
# import bpy
# import sys
#
# basedir =os.path.dirname(bpy.data.filepath)
# if basedir not in sys.path:
#     sys.path.append(basedir)

class VoxelModel():

    def __init__(self, vertices, textures, normals, faces):
        self.V = vertices
        self.T = textures
        self.N = normals
        self.F = faces


        xCoord = [x for [x,z,y] in self.V]
        yCoord = [y for [x,z,y] in self.V]
        minX = min(xCoord)
        minY = min(yCoord)
        maxX = max(xCoord)
        maxY = max(yCoord)
        self.topLeftCorner = (minX, minY)
        self.lowerRightCorner = (maxX, maxY)

        self.blocks = {}
        self.colors = {}

        #calculate z coordinates and voxel block size
        z = set([x[1] for x in self.V])
        sortedLayers = list(z)
        sortedLayers.sort()
        self.Z = sortedLayers
        self.voxelSize = (self.Z[-1] - self.Z[0])/(len(self.Z)-1)

        self.width = int(round((maxX - minX) / self.voxelSize))
        self.height = int(round((maxY - minY) / self.voxelSize))

        # for future lookup of vertex index by coordinates
        self.coordToIndex = {}
        for i in range(len(self.V)):
            self.coordToIndex[tuple(self.V[i])] = i

        # for future lookup of face by vertices index
        verticesToFace_temp = []
        for face in self.F:
            quad = [face[0][0], face[1][0], face[2][0], face[3][0]]
            quad.sort()
            verticesToFace_temp.append(tuple(quad))
        self.verticesToFace = set(verticesToFace_temp)


    def organize_vertices_by_z(self):
        verticesOrganizedByZ = {z: set([]) for z in self.Z}
        for [x, z, y] in self.V:
            verticesOrganizedByZ[z].add((x, y))
        return verticesOrganizedByZ

    def vertices_per_layer(self, layeriZ, verticesOnLayeri, layerjZ, veticesOnLayerj):
        validCoord = set([])
        validFaces = set([])
        validEdges = []
        # keep only vertices that appear in both layers
        validCoordPreliminary = [(x,y) for (x,y) in verticesOnLayeri if (x, y) in veticesOnLayerj]
        #validCoordPreliminary = list(filter((lambda (x, y): (x, y) in veticesOnLayerj), verticesOnLayeri))
        sorted_vertical_validCoordPreliminary = sorted(validCoordPreliminary, key=lambda x: (x[0], x[1]))
        sorted_horizontal_validCoordPreliminary = sorted(validCoordPreliminary, key=lambda x: (x[1], x[0]))

        # filter vertices connected by a vertical face between two layers
        for i in range(len(sorted_vertical_validCoordPreliminary) - 1):
            # if two vertices on the same x plane, check if they are connected by a face
            if sorted_vertical_validCoordPreliminary[i][0] == sorted_vertical_validCoordPreliminary[i + 1][0]:
                (x1, y1) = sorted_vertical_validCoordPreliminary[i]
                (x2, y2) = sorted_vertical_validCoordPreliminary[i + 1]
                index1 = self.coordToIndex[(x1, layeriZ, y1)]
                index2 = self.coordToIndex[(x1, layerjZ, y1)]
                index3 = self.coordToIndex[(x2, layeriZ, y2)]
                index4 = self.coordToIndex[(x2, layerjZ, y2)]
                faceVertices = [index1, index2, index3, index4]
                faceVertices.sort()
                if tuple(faceVertices) in self.verticesToFace:
                    validCoord.add((x1, y1))
                    validCoord.add((x2, y2))
                    validFaces.add(tuple(faceVertices))
                    validEdges.append(((x1,y1), (x2,y2)))

        # filter vertices connected by a horizontal face between two layers
        for i in range(len(sorted_horizontal_validCoordPreliminary) - 1):
            # if two vertices on the same x plane, check if they are connected by a face
            if sorted_horizontal_validCoordPreliminary[i][1] == sorted_horizontal_validCoordPreliminary[i + 1][1]:
                (x1, y1) = sorted_horizontal_validCoordPreliminary[i]
                (x2, y2) = sorted_horizontal_validCoordPreliminary[i + 1]
                index1 = self.coordToIndex[(x1, layeriZ, y1)]
                index2 = self.coordToIndex[(x1, layerjZ, y1)]
                index3 = self.coordToIndex[(x2, layeriZ, y2)]
                index4 = self.coordToIndex[(x2, layerjZ, y2)]
                faceVertices = [index1, index2, index3, index4]
                faceVertices.sort()
                if tuple(faceVertices) in self.verticesToFace:
                    validCoord.add((x1, y1))
                    validCoord.add((x2, y2))
                    validFaces.add(tuple(faceVertices))

        return validCoord, validFaces, validEdges

    def grid_per_layer(self, validCoord, validFaces, layeriZ, layerjZ, validEdges):
        blockContents = [[False for i in range(self.width)] for j in range(self.height)]
        minX = self.topLeftCorner[0]
        minY = self.topLeftCorner[1]

        # stores where each vertical edge is on a layer
        edges = [[] for j in range(self.height)]
        for ((x1,y1), (x2,y2)) in validEdges:
            i = int(round((x1-minX)/self.voxelSize))
            j = int(round((y1-minY)/self.voxelSize))
            edges[j].append(i)

        # a block is enclosed if there are odd edges to its right
        for j in range(self.height):
            for i in range(self.width):
                blockContents[j][i] = len([x for x in edges[j] if x > i])%2

        return blockContents

    def determine_exposed(self, blockContentsBottom, blockContentsMiddle, blockContentsTop, layer):
        #layer = int(round((layerZ - self.Z[0]) / self.voxelSize))
        exposed = [[-1 for i in range(self.width)] for j in range(self.height)]
        for j in range(self.height):
            for i in range(self.width):
                if blockContentsMiddle[j][i] == 1:
                    if i in range(1, self.width - 1) and j in range(1, self.height - 1) \
                            and blockContentsMiddle[j - 1][i] and blockContentsMiddle[j + 1][i] and blockContentsMiddle[j][i - 1] and blockContentsMiddle[j][i+1]\
                                and (blockContentsBottom[j][i]) and (blockContentsTop[j][i]):
                        exposed[j][i] = False
                    else:
                        exposed[j][i] = True

        self.blocks[layer] = [x[:] for x in exposed]
        return exposed

    def determine_exposed_bottom(self, blockContentsMiddle, blockContentsTop):
        #layer = int(round((layerZ - self.Z[0]) / self.voxelSize))
        exposed = [[-1 for i in range(self.width)] for j in range(self.height)]
        for j in range(self.height):
            for i in range(self.width):
                if blockContentsMiddle[j][i] == 1:
                    if i in range(1, self.width - 1) and j in range(1, self.height - 1) \
                            and blockContentsMiddle[j - 1][i] and blockContentsMiddle[j + 1][i] and blockContentsMiddle[j][i - 1] and blockContentsMiddle[j][i+1]\
                                and (blockContentsTop[j][i]):
                        exposed[j][i] = False
                    else:
                        exposed[j][i] = True

        self.blocks[0] = [x[:] for x in exposed]
        return exposed

    def determine_exposed_top(self, blockContentsMiddle):
        layer = len(self.Z)-2
        exposed = [[-1 for i in range(self.width)] for j in range(self.height)]
        for j in range(self.height):
            for i in range(self.width):
                if blockContentsMiddle[j][i] == 1:
                    exposed[j][i] = True
        self.blocks[layer] = [x[:] for x in exposed]

    def determine_color_per_layer(self, blockContents, numColor, layerZ, exposed):
        blockColors = [[-1 for i in range(self.width)] for j in range(self.height)]
        layer = int(round((layerZ-self.Z[0])/self.voxelSize))
        offset = layer%numColor

        #assign color to each block: "" if empty, numColor if inside, 0~numColor-1 for edge pieces
        for j in range(self.height):
            for i in range(self.width):
                if blockContents[j][i] == 0:
                    blockColors[j][i] = 0
                else:
                    #determine if it's exposed: assign alternate color
                    if exposed[j][i]:
                        blockColors[j][i] = (j*self.width+i+offset)%numColor+1
                    else:
                        blockColors[j][i] = numColor+1
        return blockColors

    def determine_top_color(self, blockContents, numColor, layerZ):
        blockColors = [[-1 for i in range(self.width)] for j in range(self.height)]
        layer = int(round((layerZ-self.Z[0])/self.voxelSize)) # zero indexed
        offset = layer%numColor

        #assign color to each block: "" if empty, numColor if inside, 0~numColor-1 for edge pieces
        for j in range(self.height):
            for i in range(self.width):
                if blockContents[j][i] == 0:
                    blockColors[j][i] = 0
                else:
                    blockColors[j][i] = (j*self.width+i+offset)%numColor+1

        self.colors[layer] = [x[:] for x in blockColors]
        return blockColors

    def determine_bottom_color(self, blockContents, numColor, layerZ):
        blockColors = [[-1 for i in range(self.width)] for j in range(self.height)]
        layer = 0
        offset = layer % numColor

        # assign color to each block: "" if empty, numColor if inside, 0~numColor-1 for edge pieces
        for j in range(self.height):
            for i in range(self.width):
                if blockContents[j][i] == 0:
                    blockColors[j][i] = 0
                else:
                    if i in range(1, self.width - 1) and j in range(1, self.height - 1)\
                            and blockContents[j - 1][i] and blockContents[j + 1][i] \
                            and blockContents[j][i - 1] and blockContents[j][i + 1]:
                        blockColors[j][i] = numColor+1
                    else:
                        blockColors[j][i] = (j * self.width + i + offset) % numColor + 1

        self.colors[layer] = [x[:] for x in blockColors]
        return blockColors

    def determine_color_whole_model(self, numColor):
        numLayers = len(self.Z) - 1
        for l in range(1,numLayers-1): # top and bottom level already colored
            self.colors[l] = [[0 for i in range(self.width)] for j in range(self.height)]
        for l in range(numLayers-2, 0, -1): # top and bottom level already colored
            for j in range(self.height):
                for i in range(self.width):
                    if self.blocks[l][j][i] == -1:
                        continue
                    elif self.blocks[l][j][i] == False:
                        self.colors[l][j][i] = numColor + 1
                    else:
                        # check adjacent blocks
                        colorCount = [0 for color in range(numColor)]
                        for (dx, dy, dz) in [(0,0,1), (0,0,-1),(1,0,0),(-1,0,0),(0,1,0),(0,-1,0)]:
                            if (l+dz) in range(numLayers) and (j+dy) in range(self.height) and (i+dx) in range(self.width):
                                color = self.colors[l+dz][j+dy][i+dx]
                                if color != 0 and color != numColor+1:
                                    colorCount[color-1] += 2
                        # check edge touching blocks
                        for (dx, dy, dz) in [(1,0,1),(-1,0,1),(0,1,1),(0,-1,1),
                                    (1,1,0),(1,-1,0),(-1,1,0),(-1,-1,0),
                                    (1,0,-1),(-1,0,-1),(0,1,-1),(0,-1,-1)]:
                            if (l+dz) in range(numLayers) and (j+dy) in range(self.height) and (i+dx) in range(self.width):
                                color = self.colors[l+dz][j+dy][i+dx]
                                if color != 0 and color!=numColor+1:
                                    colorCount[color-1] += 1
                        if sum(colorCount) != 0:
                            minValue = min(colorCount)
                            if colorCount.count(minValue) == 1:
                                assignedColor = colorCount.index(minValue)+1
                                self.colors[l][j][i] = assignedColor
                            else:
                                offset = l % numColor
                                default = (j*self.width+i+offset)%numColor
                                if colorCount[default] == minValue:
                                    self.colors[l][j][i] = default+1
                                else:
                                    self.colors[l][j][i] = colorCount.index(minValue) +1

                        else:
                            offset = l % numColor
                            self.colors[l][j][i] = (j*self.width+i+offset)%numColor+1

        #print self.colors
        return self.colors

    def color_block_left_and_right(self, j, k, blockSize,depth, color, color2, numColor):  # i = width, j = height, k = layer
        numLayers = len(self.Z) - 1
        for inc_k in range(blockSize):
            if k + inc_k < numLayers:
                for inc_j in range(blockSize):
                    if j + inc_j < self.height and True in self.blocks[k + inc_k][j + inc_j]:
                        i = self.blocks[k + inc_k][j + inc_j].index(True)
                        i_invert = self.width - self.blocks[k + inc_k][j + inc_j][::-1].index(True) - 1
                        # if self.blocks[k + inc_k][j + inc_j][i]==True and self.colors[k + inc_k][j + inc_j][i] == numColor+1:
                        #     self.colors[k + inc_k][j + inc_j][i] = color
                        # if self.blocks[k + inc_k][j + inc_j][i_invert] == True and self.colors[k + inc_k][j + inc_j][i_invert] == numColor+1:
                        #     self.colors[k + inc_k][j + inc_j][i_invert] = color
                        self.colors[k + inc_k][j + inc_j][i] = color
                        self.colors[k + inc_k][j + inc_j][i_invert] = color
                        for inc_i in range(1, depth):
                            if i + inc_i < self.width and self.blocks[k + inc_k][j + inc_j][i + inc_i]!= -1:
                                if self.colors[k + inc_k][j + inc_j][i + inc_i] == numColor+1:
                                    self.colors[k + inc_k][j + inc_j][i + inc_i] = color
                            if i_invert - inc_i > 0 and self.blocks[k + inc_k][j + inc_j][i_invert - inc_i]!= -1:
                                if self.colors[k + inc_k][j + inc_j][i_invert - inc_i] == numColor+1:
                                    self.colors[k + inc_k][j + inc_j][i_invert - inc_i] = color
        return color

    def color_block_front_and_back(self, i, k, blockSize, depth, color, color2, numColor):  # i = width, j = height, k = layer
        numLayers = len(self.Z) - 1
        for inc_k in range(blockSize):
            if k + inc_k < numLayers:
                for inc_i in range(blockSize):
                    if i + inc_i < self.width:
                        column_i = [self.blocks[k + inc_k][j][i + inc_i] for j in range(self.height)]
                        if True in column_i:
                            j = column_i.index(True)
                            j_invert = self.height - column_i[::-1].index(True) - 1
                            # if self.blocks[k + inc_k][j][i + inc_i]==True and self.colors[k + inc_k][j][i + inc_i] == numColor+1:
                            #     self.colors[k + inc_k][j][i + inc_i] = color
                            # if self.blocks[k + inc_k][j_invert][i + inc_i] == True and self.colors[k + inc_k][j_invert][i + inc_i] == numColor + 1:
                            #     self.colors[k + inc_k][j][i + inc_i] = color
                            self.colors[k + inc_k][j][i + inc_i] = color
                            self.colors[k + inc_k][j_invert][i + inc_i] = color
                            for inc_j in range(1,depth):
                                if j + inc_j < self.height and self.blocks[k + inc_k][j + inc_j][i + inc_i] != -1:
                                    if self.colors[k + inc_k][j + inc_j][i + inc_i] == numColor+1:
                                        self.colors[k + inc_k][j + inc_j][i + inc_i] = color
                                if j_invert - inc_j > 0 and self.blocks[k + inc_k][j_invert - inc_j][i + inc_i] != -1:
                                    if self.colors[k + inc_k][j_invert - inc_j][i + inc_i] == numColor+1:
                                        self.colors[k + inc_k][j_invert - inc_j][i + inc_i] = color
        return color

    def color_block_top(self,i,j,blockSize, depth, color, numColor):
        numLayers = len(self.Z) - 1
        for inc_j in range(blockSize):
            if j + inc_j < self.height:
                for inc_i in range(blockSize):
                    if i + inc_i < self.width:
                        column_k = [self.blocks[k][j+inc_j][i+inc_i] for k in range(numLayers)]
                        if True in column_k:
                            k = numLayers - column_k[::-1].index(True) - 1
                            # if self.blocks[k][j + inc_j][i + inc_i] == True and self.colors[k][j + inc_j][i + inc_i] == numColor +1:
                            #     self.colors[k][j + inc_j][i + inc_i] = color
                            self.colors[k][j + inc_j][i + inc_i] = color
                            for inc_k in range(1, depth):
                                if k-inc_k > 0 and self.blocks[k-inc_k][j+inc_j][i+inc_i] != -1:
                                    if self.colors[k - inc_k][j + inc_j][i + inc_i] == numColor+1:
                                        self.colors[k - inc_k][j + inc_j][i + inc_i] = color

    def color_remaining(self, numColor, blockSize, depth):
        numLayers = len(self.Z) - 1
        for k in range(numLayers):
            for j in range(self.height):
                for i in range(self.width):
                    if self.colors[k][j][i] == numColor+1 and self.blocks[k][j][i] == True: # exposed voxel have yet colored
                        offset = (k//depth)%numColor
                        color = ((j//blockSize) * self.width + (i//blockSize) + offset) % numColor + 1
                        self.colors[k][j][i] = color

    def determine_color_blocks(self, numColor, blockSize, depth):
        numLayers = len(self.Z) - 1
        for k in range(numLayers):
            self.colors[k] = [[numColor+1 for i in range(self.width)] for j in range(self.height)]
            for j in range(self.height):
                for i in range(self.width):
                    if self.blocks[k][j][i] == -1:
                        self.colors[k][j][i] = 0
        # side front and back
        first_color = 0
        for k in range(0, numLayers, blockSize):
            color = first_color
            for i in range(0, self.width, blockSize):
                color = (color +1)%numColor
                self.color_block_front_and_back(i, k, blockSize, depth, color+1, color+1, numColor)
                if i == 0:
                    first_color = color

        # side left and right
        first_color = 0
        for k in range(0, numLayers, blockSize):
            color = first_color
            for j in range(0, self.height, blockSize):
                color = (color + 1) % numColor
                self.color_block_left_and_right(j,k,blockSize, depth, color+1, color+1, numColor)
                if j == 0:
                    first_color = color

        # side top
        first_color =0
        for j in range(0, self.height, blockSize):
            color = first_color
            for i in range(0, self.width, blockSize):
                color = (color + 1) % numColor
                self.color_block_top(i, j, blockSize, depth, color+1, numColor)
                if i == 0:
                    first_color = color

        self.color_remaining(numColor, blockSize, depth)
