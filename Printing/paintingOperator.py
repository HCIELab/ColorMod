import bpy 
from bpy.types import Operator 

class PaintingOperator(Operator): 
    bl_idname = "object.painting_operator" 
    bl_label = "Apply color to selected voxels" 
    bl_options = {'REGISTER', 'UNDO'} 
    
    color = bpy.props.StringProperty()
    
    @classmethod
    def poll(self, context): 
        return context.active_object is not None 
    
    def execute(self, context): 
        obj = context.active_object
        if self.color == "red": 
            obj.data.materials[0] = bpy.data.materials['red'] 
        elif self.color == "blue": 
            obj.data.materials[0] = bpy.data.materials['blue'] 
        return {'FINISHED'} 

def register(): 
    bpy.utils.register_class(PaintingOperator) 
    
def unregister(): 
    bpy.utils.unregister_class(PaintingOperator) 
    
if __name__ == "__main__": 
    register()        