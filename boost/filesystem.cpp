/**
**功能描述: 封装boost的文件操作相关接口实现, 用于realdb获取存储目录下的db和table
**创建者:   guorui-ren@foxmail.com
**创建时间: 2021.08.17
**github:   https://github.com/guorui-ren/demos.git
**备注：
**/

#include<boost/filesystem.hpp>
#include<iostream>
#include<string>
#include<map>
#include<vector>

std::string get_program_cur_path()
{
    return boost::filesystem::current_path().string();
}

typedef std::map<std::string, std::vector<std::string>> map_db_table;

void get_db_table(const std::string & storage_path, map_db_table & db_tables)
{
    if(storage_path.empty())
    {
        return;
    }
    
    boost::filesystem::path boost_storage_path(storage_path.c_str());
    
    if(!boost::filesystem::is_directory(boost_storage_path))
    {
        return;
    }
    
    // 数据存储时两层目录结构
    // 1层，目录文件，代表db名称
    // 2层, 普通数据文件， 代表table名称
    boost::filesystem::recursive_directory_iterator db_beg_iter(boost_storage_path);
    boost::filesystem::recursive_directory_iterator db_end_iter;
    for(; db_beg_iter != db_end_iter; db_beg_iter++)
    {
        if(boost::filesystem::is_directory(db_beg_iter->path().string()))
        {
            std::string db_path = db_beg_iter->path().string();
            
            std::size_t db_name_pos = db_path.find_last_of("/");
            std::string db_name = db_path.substr(db_name_pos + 1);
            
            boost::filesystem::recursive_directory_iterator table_beg_iter(db_path.c_str());
            boost::filesystem::recursive_directory_iterator table_end_iter;
            for(; table_beg_iter != table_end_iter; table_beg_iter++)
            {
                if(!boost::filesystem::is_directory(table_beg_iter->path().string()))
                {
                    std::size_t table_name_pos = table_beg_iter->path().string().find_last_of("/");
                    std::string table_name = table_beg_iter->path().string().substr(table_name_pos + 1);
                    db_tables[db_name].push_back(table_name);
                }
            }
        }
    }
}

int main()
{
    std::cout << "boost filesystem demo" << std::endl;
    std::cout << "pro cur path: " << get_program_cur_path().c_str() << std::endl;
    
    //print dir tree
    map_db_table dir;
    get_db_table(get_program_cur_path(), dir);
    map_db_table::iterator it = dir.begin();
    for(; it != dir.end(); it++)
    {
        std::cout << (it->first).c_str() << std::endl;
        std::vector<std::string>::iterator it_file = (it->second).begin();
        for(; it_file != (it->second).end(); it_file++)
        {
            std::cout << "  " << (*it_file).c_str() << std::endl;
        }
    }
}
