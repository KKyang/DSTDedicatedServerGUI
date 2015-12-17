#ifndef DSTDATASTRUCTURE
#define DSTDATASTRUCTURE
#include <QString>
#include <vector>

struct properties{
    properties(){}
    properties(QString g, QString n, QString s) : group(g), name(n), settings(s){}
    QString group;
    QString name;
    QString settings;
};
struct mods_configuration{
    QString name;
    QString label;
    QString settings;

    std::vector<QString> option_names;
    std::vector<QString> options;
};

struct mods_properties{
    QString id;
    QString name;
    QString description;
    QString author;
    QString version;
    QString forumthread;
    QString api_version;
    bool client_only_mod;
    bool dst_compatible;
    bool all_clients_require_mod;
    QString icon_atlas;
    QString icon;

    std::vector<mods_configuration> conf;
};

struct DSTSettings{
    QString world_name;
    std::vector<properties> ini;

    std::vector<properties> pro;
    std::vector<std::vector<QString>> pro_items;

    std::vector<QString> used_mods;
};

enum{
    DST_WORLD = 0,
    DST_CAVE = 1
};

#endif // DSTDATASTRUCTURE

