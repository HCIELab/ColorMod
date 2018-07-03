import bpy 
from bpy.types import Operator
import os
import sys 

class ConvertModelsOperator(Operator): 
    bl_idname = "object.convert_models_operator"
    bl_label = "Convert Processed OBJ Files to STL"
    bl_options = {'REGISTER', 'UNDO'}
     
    
    
    @classmethod 
    def poll(self, context):
        return True 
    
    def execute(self, context):
        path = 'C:\Program Files\Blender Foundation\Blender'
        for root, dirs, files in os.walk(path): 
            for f in files: 
                if f.endswith('.obj'): 
                    obj_file = os.path.join(path,f) 
                    stl_file = os.path.splitext(obj_file)[0]+".stl"
                    bpy.ops.object.select_all(action='SELECT') 
                    bpy.ops.object.delete()
                    bpy.ops.import_scene.obj(filepath = obj_file) 
                    bpy.ops.object.select_all(action='SELECT') 
                    bpy.ops.export_mesh.stl(filepath=stl_file)
        #for i in range(1,context.object.set_colors+1): 
            #context.scene.layers[i] = True 
            #bpy.ops.object.select_all(action='TOGGLE')
            #bpy.ops.import_scene.obj(filepath=objpath) 
            #ob = context.scene.layers[i].active_object
            #ob.layers[i] = True 
            #ob.layers = [ j==i for j in range(len(ob.layers)) ]
        return {'FINISHED'}
    
    
def register():
    bpy.utils.register_class(ConvertModelsOperator)

def unregister():
    bpy.utils.unregister_class(ConvertModelsOperator)
    
if __name__ == "__main__":
    register()
