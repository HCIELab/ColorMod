# bl_info = {
#     "name": "Voxel Object",
#     "category": "Object",
# }
# import bpy
# import os
# import bpy
# import sys
#
# basedir =os.path.dirname(bpy.data.filepath)
# if basedir not in sys.path:
#     sys.path.append(basedir)

class VoxelObj():
    def __init__(self, material):
        self.vertices = []
        self.faces = set([])
        self.material = material

    def update(self, facesSet, verticesList):
        self.faces = self.faces | facesSet
        self.vertices += verticesList

# def register():
#     bpy.utils.register_class(VoxelObj)
#
# def unregister():
#     bpy.utils.unregister_class(VoxelObj)
#
# if __name__ == "__main__":
#     register()