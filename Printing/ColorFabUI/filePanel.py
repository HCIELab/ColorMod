import bpy 

class FilePanel(bpy.types.Panel):
    """Creates a Custom Panel to Load and Save File"""
    
    bl_label = "Custom File Panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'
    
    def draw(self, context): 
        layout = self.layout
        
        # Buttons to load different files: obj, stl, fbx 
        row = layout.row()
        row.label(text="Load file")
        split = layout.split()
        col = split.column(align = True)
        
        col.operator("import_scene.obj")
        col.operator("import_mesh.stl")
        col.operator("import_scene.fbx")
        
        # Buttons to save blender file or export as stl file 
        row = layout.row()
        row.label(text="Save file")
        split = layout.split()
        col = split.column(align = True)
        
        col.operator("wm.save_as_mainfile")
        col.operator("wm.save_mainfile")
        col.operator("export_mesh.stl")
        
        
def register():
    bpy.utils.register_class(FilePanel)


def unregister():
    bpy.utils.unregister_class(FilePanel)


if __name__ == "__main__":
    register()