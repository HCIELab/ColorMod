import bpy 
from bpy.types import Operator
    
class ConvertToBWOperator(Operator):
    bl_idname = "object.convert_to_bw_operator"
    bl_label = "Convert Colored Voxels to Black"
    bl_options = {'REGISTER', 'UNDO'}
    
    @classmethod 
    def poll(self, context):
        return True 
    
    def execute(self, context):
        color = ['white', 'red', 'blue'] 
        for i in range(1,len(color)+1): 
            objects_on_current_layer = [ob for ob in bpy.context.scene.objects if ob.layers[i]]
            for obj in objects_on_current_layer: 
                if obj.data.materials[0] == bpy.data.materials[color[i]]:
                    obj.data.materials[0] = bpy.data.materials['black']
                else: 
                    obj.data.materials[0] = bpy.data.materials['white'] 
        return {'FINISHED'}
    
    
def register():
    bpy.utils.register_class(ConvertToBWOperator)

def unregister():
    bpy.utils.unregister_class(ConvertToBWOperator)

if __name__ == "__main__":
    register()

