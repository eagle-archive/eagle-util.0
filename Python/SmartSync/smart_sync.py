import os
from xml.etree import ElementTree

def getProjNameByPath(xml_projects, path):
    for project in xml_projects:
        if project.attrib['path'] == path:
            return project.attrib['name']
    return None


if __name__=="__main__":
    MIRROR_ROOT = '/proj/repo01/gerrit/'
    PROJ_FOLDER = '.repo/projects/'

    manifest_root = ElementTree.parse('.repo/manifest.xml')
    xml_projects = manifest_root.getiterator("project")

    for project in xml_projects:
        project_path = project.attrib['path']
        project_name = project.attrib['name']

        os.makedirs(PROJ_FOLDER + project_path)
        os.rmdir(PROJ_FOLDER + project_path)

        os.symlink(MIRROR_ROOT + project_name + '.git', PROJ_FOLDER + project_path + '.git')
