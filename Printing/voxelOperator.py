import bpy 
from bpy.types import Operator

class VoxelPreviewOperator(Operator): 
    bl_idname = "object.voxel_preview_operator"
    bl_label = "Add Remesh modifier and apply"
    bl_options = {'REGISTER', 'UNDO'}
    
    @classmethod 
    def poll(self, context):
        return True 
    
    def execute(self, context):
        ob = context.active_object
        if "Remesh" in ob.modifiers:
            bpy.ops.object.modifier_remove(modifier="Remesh")
        bpy.ops.object.modifier_add(type='REMESH')
        mod = ob.modifiers["Remesh"]
        mod.octree_depth = context.object.set_octree_depth 
        mod.mode = 'BLOCKS'
        mod.scale = 0.99
        mod.use_remove_disconnected = True 
        return {'FINISHED'}
    
class VoxelOperator(Operator):
    bl_idname = "object.voxel_operator"
    bl_label = "Add Remesh modifier and apply"
    bl_options = {'REGISTER', 'UNDO'}
    
    @classmethod 
    def poll(self, context):
        return context.active_object is not None and context.active_object.type == 'MESH'
    
    def execute(self, context):
        ob = context.active_object
        mod = ob.modifiers.new("", 'REMESH')
        mod.octree_depth = context.object.set_octree_depth 
        mod.mode = 'BLOCKS'
        mod.scale = 0.99
        mod.use_remove_disconnected = True 
        bpy.ops.object.modifier_apply(apply_as='DATA', modifier=mod.name)
        return {'FINISHED'}
    
    
def register():
    bpy.utils.register_class(VoxelOperator)
    bpy.utils.register_class(VoxelPreviewOperator)
    bpy.types.Object.set_octree_depth = bpy.props.IntProperty(
        name = "Octree Depth", 
        description = "Voxelize the model with selected resolution", 
        default = 5, 
        min = 1, 
        max = 12
    )

def unregister():
    bpy.utils.unregister_class(VoxelOperator)
    bpy.utils.unregister_class(VoxelPreviewOperator)
    del bpy.types.Object.set_octree_depth

    

if __name__ == "__main__":
    register()
    # test call 
    #bpy.ops.object.voxel_operator()
