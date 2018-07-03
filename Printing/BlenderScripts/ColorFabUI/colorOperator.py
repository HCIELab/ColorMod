import bpy 
from bpy.types import Operator
import os
import sys 

basedir =os.path.dirname(bpy.data.filepath)
if basedir not in sys.path:
    sys.path.append(basedir)
from VoxelObj import VoxelObj
from export_obj_by_layer import export_obj_by_layer
from load_obj import load_obj 
from export_obj import export_obj 

class ColorOperator(Operator): 
    bl_idname = "object.assign_color_operator"
    bl_label = "Assign Colors to Shell and Separate Model by Colors"
    bl_options = {'REGISTER', 'UNDO'}
    
    
    @classmethod 
    def poll(self, context):
        return True 
    
    def execute(self, context):
        OBJFile = context.object.filename 
        filename = os.path.join(basedir , OBJFile)
        voxelizeModel = load_obj(filename)
        color = context.object.set_colors
        block = context.object.block_size
        export_obj(voxelizeModel, color, block)
        return {'FINISHED'}
    
    
def register():
    bpy.utils.register_class(ColorOperator)
    bpy.types.Object.set_colors = bpy.props.IntProperty(
        name = "Number of Colors", 
        description = "Number of colors to be assigned to shell", 
        default = 2, 
        min = 1, 
        max = 4
    )
    bpy.types.Object.block_size = bpy.props.IntProperty(
        name="Number of Voxels",
        description="Number of voxels to be assigned to each color block",
        default=3,
        min=1,
        max=10
    )
    bpy.types.Object.filename = bpy.props.StringProperty(
        name = "Name of OBJ File", 
        description = "Specify the obj file of the voxelized model" 
    )


def unregister():
    bpy.utils.unregister_class(ColorOperator)
    del bpy.types.Object.set_colors
    del bpy.types.Object.block_size
    del bpy.types.Object.filename 

    

if __name__ == "__main__":
    register()
    # test call 
    #bpy.ops.object.voxel_operator()
