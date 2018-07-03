class ConvertModelsOperator(Operator): 
    bl_idname = "object.convert_models_operator"
    bl_label = "Load Processed Models to Separate Layers"
    bl_options = {'REGISTER', 'UNDO'}
     
    
    
    @classmethod 
    def poll(self, context):
        return True 
    
    def execute(self, context):
        path = 'C:\Program Files\Blender Foundation\Blender'
        i= 0 
        for root, dirs, files in os.walk(path): 
            for f in files: 
                if f.endswith('.obj'): 
                    i += 1 
                    obj_file = os.path.join(path,f) 
                    stl_file = os.path.splitext(obj_file)[0]+".stl"
                    bpy.ops.object.select_all(action='SELECT') 
                    bpy.ops.object.delete()
                    print (i) 
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
    bpy.utils.register_class(ColorOperator)
    bpy.utils.register_class(LoadModelsToLayers)
    bpy.types.Object.set_colors = bpy.props.IntProperty(
        name = "Number of Colors", 
        description = "Number of colors to be assigned to shell", 
        default = 2, 
        min = 1, 
        max = 4
    )
    bpy.types.Object.filename = bpy.props.StringProperty(
        name = "Name of OBJ File", 
        description = "Specify the obj file of the voxelized model" 
    )

def unregister():
    bpy.utils.unregister_class(ColorOperator)
    bpy.utils.unregister_class(LoadModelsToLayers)
    del bpy.types.Object.set_colors
    del bpy.types.Object.filename 

    

if __name__ == "__main__":
    register()
    # test call 
    #bpy.ops.object.voxel_operator()
