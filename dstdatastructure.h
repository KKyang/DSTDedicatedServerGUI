#ifndef DSTDATASTRUCTURE
#define DSTDATASTRUCTURE

struct properties{
    properties(){}
    properties(QString g, QString n, QString s) : group(g), name(n), settings(s){}
    QString group;
    QString name;
    QString settings;
};

struct DSTSettings{
    QString world_name;
    std::vector<properties> ini;
    std::vector<properties> pro;
};

enum{
    DST_WORLD = 0,
    DST_CAVE = 1
};

#endif // DSTDATASTRUCTURE

