import bpy 
from bpy.types import Operator

class ImportLayerOperator(Operator): 
    bl_idname = "object.import_layer_operator"
    bl_label = "Import model to a specific layer"
    bl_options = {'REGISTER', 'UNDO'}
    
    layer = bpy.props.IntProperty()
    
    @classmethod 
    def poll(self, context):
        return context.active_object is not None and context.active_object.type == 'MESH'
    
    def execute(self, context):
        ob = context.active_object
        ob.layers[self.layer] = True 
        ob.layers = [ i==self.layer for i in range(len(ob.layers)) ]
        return {'FINISHED'}
    
def register():
    bpy.utils.register_class(ImportLayerOperator)

def unregister():
    bpy.utils.unregister_class(ImportLayerOperator)
    

if __name__ == "__main__":
    register()
    # test call 
    #bpy.ops.object.voxel_operator()
