#include "misc.h"
#include "printout.h"
#include "logger.h"
#include "speedtestutil.h"

using namespace std;
using namespace rapidjson;
using namespace YAML;

string base_ss_win = "{\"version\":\"4.1.6\",\"configs\":[?config?],\"strategy\":null,\"index\":0,\"global\":false,\"enabled\":false,\"shareOverLan\":true,\"isDefault\":false,\"localPort\":?localport?,\"portableMode\":true,\"pacUrl\":null,\"useOnlinePac\":false,\"secureLocalPac\":true,\"availabilityStatistics\":false,\"autoCheckUpdate\":true,\"checkPreRelease\":false,\"isVerboseLogging\":false,\"logViewer\":{\"topMost\":false,\"wrapText\":false,\"toolbarShown\":false,\"Font\":\"Consolas, 8pt\",\"BackgroundColor\":\"Black\",\"TextColor\":\"White\"},\"proxy\":{\"useProxy\":false,\"proxyType\":0,\"proxyServer\":\"\",\"proxyPort\":0,\"proxyTimeout\":3,\"useAuth\":false,\"authUser\":\"\",\"authPwd\":\"\"},\"hotkey\":{\"SwitchSystemProxy\":\"\",\"SwitchSystemProxyMode\":\"\",\"SwitchAllowLan\":\"\",\"ShowLogs\":\"\",\"ServerMoveUp\":\"\",\"ServerMoveDown\":\"\",\"RegHotkeysAtStartup\":false}}";
string config_ss_win = "{\"server\":\"?server?\",\"server_port\":?port?,\"password\":\"?pw?\",\"method\":\"?method?\",\"plugin\":\"?plugin?\",\"plugin_opts\":\"?plugin_opts?\",\"plugin_args\":\"\",\"remarks\":\"?remarks?\",\"timeout\":5}";
string config_ss_libev = "{\"server\":\"?server?\",\"server_port\":?port?,\"password\":\"?pw?\",\"method\":\"?method?\",\"plugin\":\"?plugin?\",\"plugin_opts\":\"?plugin_opts?\",\"plugin_args\":\"\",\"local_address\":\"127.0.0.1\",\"local_port\":?localport?}";
string base_ssr_win = "{\"configs\":[?config?],\"index\":0,\"random\":true,\"sysProxyMode\":1,\"shareOverLan\":false,\"localPort\":?localport?,\"localAuthPassword\":null,\"localDnsServer\":\"\",\"dnsServer\":\"\",\"reconnectTimes\":2,\"balanceAlgorithm\":\"LowException\",\"randomInGroup\":false,\"TTL\":0,\"connectTimeout\":5,\"proxyRuleMode\":2,\"proxyEnable\":false,\"pacDirectGoProxy\":false,\"proxyType\":0,\"proxyHost\":null,\"proxyPort\":0,\"proxyAuthUser\":null,\"proxyAuthPass\":null,\"proxyUserAgent\":null,\"authUser\":null,\"authPass\":null,\"autoBan\":false,\"checkSwitchAutoCloseAll\":false,\"logEnable\":false,\"sameHostForSameTarget\":false,\"keepVisitTime\":180,\"isHideTips\":false,\"nodeFeedAutoUpdate\":true,\"serverSubscribes\":[],\"token\":{},\"portMap\":{}}";
string config_ssr_win = "{\"remarks\":\"?remarks?\",\"id\":\"18C4949EBCFE46687AE4A7645725D35F\",\"server\":\"?server?\",\"server_port\":?port?,\"server_udp_port\":0,\"password\":\"?pw?\",\"method\":\"?method?\",\"protocol\":\"?protocol?\",\"protocolparam\":\"?protoparam?\",\"obfs\":\"?obfs?\",\"obfsparam\":\"?obfsparam?\",\"remarks_base64\":\"?remarks_base64?\",\"group\":\"?group?\",\"enable\":true,\"udp_over_tcp\":false}";
string config_ssr_libev = "{\"server\":\"?server?\",\"server_port\":?port?,\"protocol\":\"?protocol?\",\"method\":\"?method?\",\"obfs\":\"?obfs?\",\"password\":\"?password?\",\"obfs_param\":\"?obfsparam?\",\"protocol_param\":\"?protoparam?\",\"local_address\":\"127.0.0.1\",\"local_port\":?localport?}";
string base_vmess = "{\"inbounds\":[{\"port\":?localport?,\"listen\":\"0.0.0.0\",\"protocol\":\"socks\"}],\"outbounds\":[{\"tag\":\"proxy\",\"protocol\":\"vmess\",\"settings\":{\"vnext\":[{\"address\":\"?add?\",\"port\":?port?,\"users\":[{\"id\":\"?id?\",\"alterId\":?aid?,\"email\":\"t@t.tt\",\"security\":\"?cipher?\"}]}]},\"streamSettings\":{\"network\":\"?net?\",\"security\":\"?tls?\",\"tlsSettings\":null,\"tcpSettings\":?tcpset?,\"wsSettings\":?wsset?},\"mux\":{\"enabled\":true}}],\"routing\":{\"domainStrategy\":\"IPIfNonMatch\"}}";
string wsset_vmess = "{\"connectionReuse\":true,\"path\":\"?path?\",\"headers\":{\"Host\":\"?host?\"}}";
string tcpset_vmess = "{\"connectionReuse\":true,\"header\":{\"type\":\"?type?\",\"request\":{\"version\":\"1.1\",\"method\":\"GET\",\"path\":[\"?path?\"],\"headers\":{\"Host\":[\"?host?\"],\"User-Agent\":[\"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.75 Safari/537.36\",\"Mozilla/5.0 (iPhone; CPU iPhone OS 10_0_2 like Mac OS X) AppleWebKit/601.1 (KHTML, like Gecko) CriOS/53.0.2785.109 Mobile/14A456 Safari/601.1.46\"],\"Accept-Encoding\":[\"gzip, deflate\"],\"Connection\":[\"keep-alive\"],\"Pragma\":\"no-cache\"}}}}";

//remake from speedtestutil

template <typename T> void operator>>(const YAML::Node& node,  T& i)
{
    i = node.as<T>();
};

string vmess_construct(string add, string port, string type, string id, string aid, string net, string cipher, string path, string host, string tls, int local_port)
{
    string base = base_vmess;
    string wsset = wsset_vmess;
    string tcpset = tcpset_vmess;
    base = replace_all_distinct(base, "?localport?", to_string(local_port));
    base = replace_all_distinct(base, "?add?", add);
    base = replace_all_distinct(base, "?port?", port);
    base = replace_all_distinct(base, "?id?", id);
    base = replace_all_distinct(base, "?aid?", aid);
    base = replace_all_distinct(base, "?net?", net);
    base = replace_all_distinct(base, "?cipher?", cipher);
    if(net == "ws")
    {
        wsset = replace_all_distinct(wsset, "?host?", host == "" ? add : host);
        wsset = replace_all_distinct(wsset, "?path?", path == "" ? "/" : path);
        base = replace_all_distinct(base, "?wsset?", wsset);
    }
    if(type == "http")
    {
        tcpset = replace_all_distinct(tcpset, "?host?", host == "" ? add : host);
        tcpset = replace_all_distinct(tcpset, "?type?", type);
        tcpset = replace_all_distinct(tcpset, "?path?", path == "" ? "/" : path);
        base = replace_all_distinct(base, "?tcpset?", tcpset);
    }
    base = replace_all_distinct(base, "?tls?", tls);
    base = replace_all_distinct(base, "?tcpset?", "null");
    base = replace_all_distinct(base, "?wsset?", "null");

    return base;
}

string ssr_construct(string group, string remarks, string remarks_base64, string server, string port, string protocol, string method, string obfs, string password, string obfsparam, string protoparam, int local_port, bool libev)
{
    string base = base_ssr_win;
    string config = config_ssr_win;
    string config_libev = config_ssr_libev;
    if(libev == true)
        config = config_libev;
    config = replace_all_distinct(config, "?group?", group);
    config = replace_all_distinct(config, "?remarks?", remarks);
    config = replace_all_distinct(config, "?remarks_base64?", remarks_base64);
    config = replace_all_distinct(config, "?server?", server);
    config = replace_all_distinct(config, "?port?", port);
    config = replace_all_distinct(config, "?protocol?", protocol);
    config = replace_all_distinct(config, "?method?", method);
    config = replace_all_distinct(config, "?obfs?", obfs);
    config = replace_all_distinct(config, "?password?", password);
    config = replace_all_distinct(config, "?obfsparam?", obfsparam);
    config = replace_all_distinct(config, "?protoparam?", protoparam);
    if(libev == true)
        base = config;
    else
        base = replace_all_distinct(base, "?config?", config);
    base = replace_all_distinct(base, "?localport?", to_string(local_port));

    return base;
}

string ss_construct(string server, string port, string pw, string method, string plugin, string pluginopts, string ps, int local_port, bool libev)
{
    string base = base_ss_win;
    string config = config_ss_win;
    string config_libev = config_ss_libev;
    if(libev == true)
        config = config_libev;
    config = replace_all_distinct(config, "?server?", server);
    config = replace_all_distinct(config, "?port?", port);
    config = replace_all_distinct(config, "?pw?", pw);
    config = replace_all_distinct(config, "?method?", method);
    config = replace_all_distinct(config, "?plugin?", plugin);
    config = replace_all_distinct(config, "?plugin_opts?", pluginopts);
    config = replace_all_distinct(config, "?remarks?", ps);
    if(libev == true)
        base = config;
    else
        base = replace_all_distinct(base, "?config?", config);
    base = replace_all_distinct(base, "?localport?", to_string(local_port));

    return base;
}

void explodevmess(string vmess, string custom_port, int local_port, nodeInfo *node)
{
    string ps, add, port, type, id, aid, net, path, host, tls;
    Document jsondata;
    vmess = vmess.substr(8);
    vmess = base64_decode(vmess);
    if(regmatch(vmess, "(.*?) = (.*)"))
    {
        explodequan(vmess, custom_port, local_port, node);
        return;
    }
    jsondata.Parse(vmess.data());
    ps = jsondata["ps"].GetString();
    add = jsondata["add"].GetString();
    port = custom_port == ""?jsondata["port"].GetString():custom_port;
    path = jsondata["path"].GetString();
    type = jsondata["type"].GetString();
    id = jsondata["id"].GetString();
    if(jsondata["aid"].IsInt())
        aid = to_string(jsondata["aid"].GetInt());
    else
        aid = jsondata["aid"].GetString();
    net = jsondata["net"].GetString();
    host = jsondata["host"].GetString();
    tls = jsondata["tls"].GetString();

    //cout<<"vmess, V2RayProvider, "<<UTF8ToGBK(ps)<<", "<<add<<", "<<port<<", ";
    //cout<<base<<endl;
    node->linktype = SPEEDTEST_MESSAGE_FOUNDVMESS;
    node->group = "V2rayProvider";
    node->remarks = ps;
    node->server = add;
    node->port = stoi(port);
    node->proxystr = vmess_construct(add, port, type, id, aid, net, "auto", path, host, tls, local_port);
}

void explodessr(string ssr, bool libev, string custom_port, int local_port, nodeInfo *node)
{
    string strobfs;
    vector<string> strcfg;
    string remarks = "", group = "", server, port, method, pw, protocol, protoparam = "", obfs, obfsparam = "", remarks_base64;
    ssr = ssr.substr(6);
    ssr = urlsafe_base64_decode(ssr);
    ssr = regreplace(ssr, "(.*):(.*?):(.*?):(.*?):(.*?):(.*?)\\/\\?(.*)", "$1,$2,$3,$4,$5,$6,$7");
    strcfg = split(ssr,",");
    strobfs = strcfg[6];
    group = urlsafe_base64_decode(getUrlArg(strobfs, "group"));
    if(group == "")
        group = "SSRProvider";
    remarks = urlsafe_base64_decode(getUrlArg(strobfs, "remarks"));
    remarks_base64 = urlsafe_base64_reverse(getUrlArg(strobfs, "remarks"));
    server = strcfg[0];
    if(remarks == "")
    {
        remarks = server;
        remarks_base64 = base64_encode(remarks);
    }
    port = custom_port == ""?strcfg[1]:custom_port;
    protocol = strcfg[2];
    method = strcfg[3];
    obfs = strcfg[4];
    pw = base64_decode(strcfg[5]);
    obfsparam = base64_decode(getUrlArg(strobfs, "obfsparam"));
    protoparam = base64_decode(getUrlArg(strobfs, "protoparam"));

    node->linktype = SPEEDTEST_MESSAGE_FOUNDSSR;
    node->group = group;
    node->remarks = remarks;
    node->server = server;
    node->port = stoi(port);
    node->proxystr = ssr_construct(group, remarks, remarks_base64, server, port, protocol, method, obfs, pw, obfsparam, protoparam, local_port, libev);
}

void explodess(string ss, bool libev, string custom_port, int local_port, nodeInfo *node)
{
    string ps = "", pw, method, server, port, plugins = "", plugin = "", pluginopts = "", addition, group = "SSProvider";
    string strTemp;
    if(strfind(ss, "#"))
    {
        ps = UrlDecode(ss.substr(ss.find("#")+1));
        ss = ss.substr(0, ss.find("#"));
    }
    if(!strfind(ss, "/?"))
    {
        ss = ss.substr(5);
        ss = base64_decode(ss);
        strTemp = regreplace(ss, "(.*?):(.*?)@(.*?):(.*?)", "$1,$2,$3,$4");
        vector<string> args = split(strTemp, ",");
        method = args[0];
        pw = args[1];
        server = args[2];
        port = custom_port == "" ? args[3] : custom_port;
    }
    else
    {
        strTemp = regreplace(ss, "ss://(.*?)@(.*?):(.*?)/\\?(.*)", "$1,$2,$3,$4");
        vector<string> args = split(strTemp, ",");
        vector<string> pass = split(urlsafe_base64_decode(args[0]), ":");
        method = pass[0];
        pw = pass[1];
        server = args[1];
        port = custom_port == "" ? args[2] : custom_port;
        addition = args[3];
        plugins = UrlDecode(getUrlArg(addition, "plugin"));
        plugin = plugins.substr(0, plugin.find(";"));
        pluginopts = plugins.substr(plugin.find(";") + 1);
        if(getUrlArg(addition, "group") != "")
            group = base64_decode(getUrlArg(addition, "group"));
    }
    if(ps == "")
        ps = server;

    node->linktype = SPEEDTEST_MESSAGE_FOUNDSS;
    node->group = group;
    node->remarks = ps;
    node->server = server;
    node->port = stoi(port);
    node->proxystr = ss_construct(server, port, pw, method, plugin, pluginopts, ps, local_port, libev);
}

void explodequan(string quan, string custom_port, int local_port, nodeInfo *node)
{
    string strTemp, itemname, itemval;
    vector<string> configs, vchild;
    strTemp = regreplace(quan, "(.*?) = (.*)", "$1,$2");
    configs = split(strTemp, ",");
    if(configs[1] == "vmess")
    {
        string group, ps, add, port, cipher = "auto", type = "none", id, aid = "0", net = "tcp", path, host, tls;
        ps = replace_all_distinct(configs[0], " ", "");
        add = replace_all_distinct(configs[2], " ", "");
        port = replace_all_distinct(configs[3], " ", "");
        cipher = replace_all_distinct(configs[4], " ", "");
        id = replace_all_distinct(replace_all_distinct(configs[5], "\"", ""), " ", "");

        //read link
        for(unsigned int i = 6; i<configs.size(); i++)
        {
            vchild = split(configs[i], "=");
            if(vchild.size()<2)
                continue;
            itemname = replace_all_distinct(vchild[0], " ", "");
            itemval = vchild[1];
            if(itemname == "group")
                group = itemval;
            else if(itemname == "over-tls")
                tls = itemval == "true" ? "tls" : "";
            else if(itemname == "tls-host")
                host = itemval;
            else if(itemname == "obfs-path")
                path = replace_all_distinct(itemval, "\"", "");
            else if(itemname == "obfs-header")
            {
                vector<string> headers = split(replace_all_distinct(replace_all_distinct(itemval, "\"", ""), "[Rr][Nn]", "|"), "|");
                for(unsigned int j = 0; j<headers.size(); j++)
                {
                    if(strfind(headers[j], "Host: "))
                        host = headers[j].substr(6);
                }
            }
            else if(itemname == "obfs")
            {
                if(itemval == "ws")
                    net = "ws";
            }
        }
        if(path == "")
            path = "/";

        node->linktype = SPEEDTEST_MESSAGE_FOUNDVMESS;
        node->group = group;
        node->remarks = ps;
        node->server = add;
        node->port = stoi(port);
        node->proxystr = vmess_construct(add, port, type, id, aid, net, cipher, path, host, tls, local_port);
    }
}

bool explodesurge(string surge, string custom_port, int local_port, vector<nodeInfo> *nodes, bool libev)
{
    string line, remarks, server, port, method, password;
    stringstream data;
    vector<string> configs;
    nodeInfo node;
    bool isSurgeConfig = false, isProxySection = false;

    data<<surge;
    while(getline(data,line))
    {
        if(!line.size())
            continue;
        else if(isProxySection)
        {
            if(regmatch(line, "(.*?) = (.*)"))
            {
                line = regreplace(line, "(.*?) = (.*)", "$1,$2");
                configs = split(line, ",");
                if(configs[1] == "custom"/* && strfind(configs[6], "ss.module")*/)
                {
                    remarks = configs[0];
                    server = replace_all_distinct(configs[2], " ", "");
                    port = custom_port == "" ? replace_all_distinct(configs[3], " ", "") : custom_port;
                    method = replace_all_distinct(configs[4], " ", "");
                    password = replace_all_distinct(configs[5], " ", "");

                    node.linktype = SPEEDTEST_MESSAGE_FOUNDSS;
                    node.group = "SSProvider";
                    node.remarks = remarks;
                    node.server = server;
                    node.port = stoi(port);
                    node.proxystr = ss_construct(server, port, password, method, "", "", remarks, local_port, libev);
                    nodes->push_back(node);
                }
            }
            else
            {
                break;
            }
        }
        else if(strfind(line, "[Proxy]") && !isProxySection)
        {
            isProxySection = true;
            isSurgeConfig = true;
        }
    }
    return isSurgeConfig;
}

void explodeclash(Node yamlnode, string custom_port, int local_port, vector<nodeInfo> *nodes, bool libev)
{
    nodeInfo node;
    int index = 0;
    string proxytype, strTemp, ps, server, port, cipher, group;
    for(unsigned int i = 0; i<yamlnode["Proxy"].size(); i++)
    {
        yamlnode["Proxy"][i]["type"]>>proxytype;
        yamlnode["Proxy"][i]["name"]>>ps;
        yamlnode["Proxy"][i]["server"]>>server;
        port = custom_port == "" ? yamlnode["Proxy"][i]["port"].as<string>() : custom_port;
        if(proxytype == "vmess")
        {
            string type = "none", id, aid = "0", net = "tcp", path, host, tls = "";
            group = "V2RayProvider";

            yamlnode["Proxy"][i]["uuid"]>>id;
            yamlnode["Proxy"][i]["alterId"]>>aid;
            yamlnode["Proxy"][i]["cipher"]>>cipher;
            net = yamlnode["Proxy"][i]["network"].IsDefined() ? yamlnode["Proxy"][i]["network"].as<string>() : "tcp";
            path = yamlnode["Proxy"][i]["ws-path"].IsDefined() ? yamlnode["Proxy"][i]["ws-path"].as<string>() : "/";
            if(yamlnode["Proxy"][i]["tls"].IsDefined())
            {
                tls = yamlnode["Proxy"][i]["tls"].as<string>() == "true" ? "tls" : "";
            }
            if(yamlnode["Proxy"][i]["ws-headers"].IsDefined())
                yamlnode["Proxy"][i]["ws-headers"]["Host"]>>host;

            node.linktype = SPEEDTEST_MESSAGE_FOUNDVMESS;
            node.proxystr = vmess_construct(server, port, type, id, aid, net, cipher, path, host, tls, local_port);
        }
        else if(proxytype == "ss")
        {
            string pw, method, plugin = "", pluginopts = "", addition;
            string pluginopts_mode, pluginopts_host;
            group = "SSProvider";

            yamlnode["Proxy"][i]["cipher"]>>method;
            yamlnode["Proxy"][i]["password"]>>pw;
            if(yamlnode["Proxy"][i]["plugin"].IsDefined())
            {
                if(yamlnode["Proxy"][i]["plugin"].as<string>() == "obfs")
                {
                    plugin = "obfs-local";
                    if(yamlnode["Proxy"][i]["plugin-opts"].IsDefined())
                    {
                        yamlnode["Proxy"][i]["plugin-opts"]["mode"]>>pluginopts_mode;
                        if(yamlnode["Proxy"][i]["plugin-opts"]["host"].IsDefined())
                            yamlnode["Proxy"][i]["plugin-opts"]["host"]>>pluginopts_host;
                    }
                }
            }
            if(plugin != "")
            {
                pluginopts = "obfs=" + pluginopts_mode;
                pluginopts += pluginopts_host == "" ? "" : ";obfs-host=" + pluginopts_host;
            }

            node.linktype = SPEEDTEST_MESSAGE_FOUNDSS;
            node.proxystr = ss_construct(server, port, pw, method, plugin, pluginopts, ps, local_port, libev);
        }
        node.group = group;
        node.remarks = ps;
        node.server = server;
        node.port = stoi(port);
        node.id = index;
        nodes->push_back(node);
        index++;
    }
    return;
}

bool chkignore(nodeInfo *node, vector<string> *exclude_remarks, vector<string> *include_remarks)
{
    bool excluded = false, included = false;
    string remarks = UTF8ToGBK(node->remarks), retremark;
    unsigned i;
    writelog(LOG_TYPE_INFO, "Comparing exclude remarks...");
    for(i = 0; i < exclude_remarks->size(); i++)
    {
        retremark = (*exclude_remarks)[i];
        if(strfind(remarks, retremark))
            excluded = true;
    }
    writelog(LOG_TYPE_INFO, "Comparing include remarks...");
    for(i = 0; i < include_remarks->size(); i++)
    {
        retremark = (*include_remarks)[i];
        if(strfind(remarks, retremark))
            included = true;
    }
    if(include_remarks->size() == 0)
        included = true;
    if(!excluded && included)
        return false;
    else
        return true;
}

void explodessconf(string content, string custom_port, int local_port, bool libev, vector<nodeInfo> *nodes)
{
    nodeInfo node;
    Document json;
    string config, ps = "", pw, method, server, port, plugin = "", pluginopts = "", group = "SSProvider";

    json.Parse(content.data());
    for(unsigned int i = 0; i<json["configs"].Size(); i++)
    {
        config = config_ss_libev;
        ps = json["configs"][i]["remarks"].GetString();
        pw = json["configs"][i]["password"].GetString();
        method = json["configs"][i]["method"].GetString();
        server = json["configs"][i]["server"].GetString();
        port = custom_port == ""?to_string(json["configs"][i]["server_port"].GetInt()):custom_port;
        plugin = json["configs"][i]["plugin"].GetString();
        pluginopts = json["configs"][i]["plugin_opts"].GetString();

        node.linktype = SPEEDTEST_MESSAGE_FOUNDSS;
        node.group = group;
        node.remarks = ps;
        node.server = server;
        node.port = stoi(port);
        node.proxystr = ss_construct(server, port, pw, method, plugin, pluginopts, ps, local_port, libev);
        nodes->push_back(node);
    }
    return;
}

void explodessrconf(string content, string custom_port, int local_port, bool libev, vector<nodeInfo> *nodes)
{
    nodeInfo node;
    Document json;
    string base, config, remarks = "", remarks_base64 = "", group = "", server, port, method, pw, protocol, protoparam = "", obfs, obfsparam = "";

    json.Parse(content.data());
    for(unsigned int i = 0; i<json["configs"].Size(); i++)
    {
        config = config_ssr_libev;
        group = json["configs"][i]["group"].GetString();
        if(group == "")
            group = "SSRProvider";
        remarks = json["configs"][i]["remarks"].GetString();
        remarks_base64 = json["configs"][i]["remarks_base64"].GetString();
        pw = json["configs"][i]["password"].GetString();
        method = json["configs"][i]["method"].GetString();
        server = json["configs"][i]["server"].GetString();
        port = custom_port == ""?to_string(json["configs"][i]["server_port"].GetInt()):custom_port;
        protocol = json["configs"][i]["protocol"].GetString();
        protoparam = json["configs"][i]["protocolparam"].GetString();
        obfs = json["configs"][i]["obfs"].GetString();
        obfsparam = json["configs"][i]["obfsparam"].GetString();

        node.linktype = SPEEDTEST_MESSAGE_FOUNDSSR;
        node.group = group;
        node.remarks = remarks;
        node.server = server;
        node.port = stoi(port);
        node.proxystr = ssr_construct(group, remarks, remarks_base64, server, port, protocol, method, obfs, pw, obfsparam, protoparam, local_port, libev);
        nodes->push_back(node);
    }
    return;
}

void explodevmessconf(string content, string custom_port, int local_port, vector<nodeInfo> *nodes)
{
    nodeInfo node;
    Document json;
    string group, ps, add, port, type, id, aid, net, path, host, tls, cipher, subid;
    map<string, string> subdata;
    map<string, string>::iterator iter;

    json.Parse(content.data());
    //read all subscribe remark as group name
    for(unsigned int i = 0; i < json["subItem"].Size(); i++)
    {
        subdata.insert(pair<string, string>(json["subItem"][i]["id"].GetString(), json["subItem"][i]["remarks"].GetString()));
    }

    for(unsigned int i = 0; i < json["vmess"].Size(); i++)
    {
        if(json["vmess"][i]["configType"].GetInt() != 1)
            continue;
        ps = json["vmess"][i]["remarks"].GetString();
        add = json["vmess"][i]["address"].GetString();
        port = custom_port == "" ? to_string(json["vmess"][i]["port"].GetInt()) : custom_port;
        type = json["vmess"][i]["headerType"].GetString();
        id = json["vmess"][i]["id"].GetString();
        aid = to_string(json["vmess"][i]["alterId"].GetInt());
        net = json["vmess"][i]["network"].GetString();
        path = json["vmess"][i]["path"].GetString();
        host = json["vmess"][i]["requestHost"].GetString();
        tls = json["vmess"][i]["streamSecurity"].GetString();
        cipher = json["vmess"][i]["security"].GetString();
        subid = json["vmess"][i]["subid"].GetString();
        if(subid != "")
        {
            iter = subdata.find(subid);
            group = iter != subdata.end() ? iter->second : "V2rayProvider";
        }
        else
            group = "V2rayProvider";

        node.linktype = SPEEDTEST_MESSAGE_FOUNDVMESS;
        node.group = group;
        node.remarks = ps;
        node.server = add;
        node.port = stoi(port);
        node.proxystr = vmess_construct(add, port, type, id, aid, net, cipher, path, host, tls, local_port);
        nodes->push_back(node);
    }
    return;
}

int explodeconf(string filepath, string custom_port, int local_port, bool sslibev, bool ssrlibev, vector<nodeInfo> *nodes, vector<string> *exclude_remarks, vector<string> *include_remarks)
{
    ifstream infile;
    string strTemp, content;
    infile.open(filepath);

    while(getline(infile, strTemp))
        content+=strTemp+"\n";

    return explodeconfcontent(content, custom_port, local_port, sslibev, ssrlibev, nodes, exclude_remarks, include_remarks);
}

int explodeconfcontent(string content, string custom_port, int local_port, bool sslibev, bool ssrlibev, vector<nodeInfo> *nodes, vector<string> *exclude_remarks, vector<string> *include_remarks)
{
    int index = 0, filetype = -1;
    if(explodesurge(content, custom_port, local_port, nodes, sslibev))
    {
        return SPEEDTEST_ERROR_NONE;
    }
    try
    {
        Node yamlnode = Load(content);
        if(yamlnode.size()&&yamlnode["Proxy"])
        {
            explodeclash(yamlnode, custom_port, local_port, nodes, sslibev);
            return SPEEDTEST_ERROR_NONE;
        }
    }
    catch (exception &e)
    {
        //ignore
    }
    if(strfind(content, "\"version\""))
        filetype = SPEEDTEST_MESSAGE_FOUNDSS;
    else if(strfind(content, "\"serverSubscribes\""))
        filetype = SPEEDTEST_MESSAGE_FOUNDSSR;
    else if(strfind(content, "\"inbound\""))
        filetype = SPEEDTEST_MESSAGE_FOUNDVMESS;

    switch(filetype)
    {
    case SPEEDTEST_MESSAGE_FOUNDSS:
        explodessconf(content, custom_port, local_port, sslibev, nodes);
        break;
    case SPEEDTEST_MESSAGE_FOUNDSSR:
        explodessrconf(content, custom_port, local_port, ssrlibev, nodes);
        break;
    case SPEEDTEST_MESSAGE_FOUNDVMESS:
        explodevmessconf(content, custom_port, local_port, nodes);
        break;
    default:
        return SPEEDTEST_ERROR_UNRECOGFILE;
    }

    for(unsigned int i = 0; i<nodes->size(); i++)
    {
        if(chkignore(&(*nodes)[i], exclude_remarks, include_remarks))
        {
            writelog(LOG_TYPE_INFO, "Node  "+(*nodes)[i].group+" - "+(*nodes)[i].remarks+"  has been ignored and will not be tested.");
            nodes->erase(nodes->begin()+i);
        }
        else
        {
            writelog(LOG_TYPE_INFO, "Node  "+(*nodes)[i].group+" - "+(*nodes)[i].remarks+"  has been added.");
            (*nodes)[i].id = index;
            index++;
        }
    }

    return SPEEDTEST_ERROR_NONE;
}

void explode(string link, bool sslibev, bool ssrlibev, string custom_port, int local_port, nodeInfo *node)
{
    if(strfind(link, "ssr://"))
        explodessr(link, ssrlibev, custom_port, local_port, node);
    else if(strfind(link, "vmess://"))
        explodevmess(link, custom_port, local_port, node);
    else if(strfind(link, "ss://"))
        explodess(link, sslibev, custom_port, local_port, node);
}

void explodessd(string link, bool libev, string custom_port, int local_port, vector<nodeInfo> *nodes)
{
    Document jsondata;
    nodeInfo node;
    string group, port, method, password, server, remarks;
    string plugin, pluginopts;
    link = base64_decode(link.substr(6));
    jsondata.Parse(link.c_str());
    for(unsigned int i = 0; i<jsondata["servers"].Size(); i++)
    {
        group = jsondata["airport"].GetString();
        port = to_string(jsondata["port"].GetInt());
        method = jsondata["encryption"].GetString();
        password = jsondata["password"].GetString();
        remarks = jsondata["servers"][i]["remarks"].GetString();
        server = jsondata["servers"][i]["server"].GetString();
        if(jsondata["servers"][i].HasMember("plugin"))
            plugin = jsondata["servers"][i]["plugin"].GetString();
        else
            plugin = "";
        if(jsondata["servers"][i].HasMember("plugin_options"))
            pluginopts = jsondata["servers"][i]["plugin_options"].GetString();
        else
            pluginopts = "";
        if(jsondata["servers"][i].HasMember("encryption"))
            method = jsondata["servers"][i]["encryption"].GetString();
        if(jsondata["servers"][i].HasMember("port"))
            port = to_string(jsondata["servers"][i]["port"].GetInt());
        if(jsondata["servers"][i].HasMember("password"))
            password = jsondata["password"][i]["password"].GetString();
        if(custom_port != "")
            port = custom_port;

        node.linktype = SPEEDTEST_MESSAGE_FOUNDSS;
        node.group = group;
        node.remarks = remarks;
        node.server = server;
        node.port = stoi(port);
        node.proxystr = ss_construct(server, port, password, method, plugin, pluginopts, remarks, local_port, libev);
        node.id = i;
        nodes->push_back(node);
        //nodes->insert(nodes->end(), node);
    }
    return;
}

void explodesub(string sub, bool sslibev, bool ssrlibev, string custom_port, int local_port, vector<nodeInfo> *nodes, vector<string> *exclude_remarks, vector<string> *include_remarks)
{
    stringstream strstream;
    string strLink;
    nodeInfo node;

    //try to parse as surge configuration
    if(explodesurge(sub, custom_port, local_port, nodes, sslibev))
    {
        return;
    }

    //try to parse as clash configuration
    try
    {
        Node yamlnode = Load(sub);
        if(yamlnode.size() && yamlnode["Proxy"])
        {
            explodeclash(yamlnode, custom_port, local_port, nodes, sslibev);
            return;
        }
    }
    catch (exception &e)
    {
        //ignore
    }

    //try to parse as SSD configuration
    if(strfind(sub, "ssd://"))
    {
        explodessd(sub, sslibev, custom_port, local_port, nodes);
        return;
    }

    //try to parse as normal subscription
    sub = base64_decode(sub);
    strstream<<sub;
    int index = 0;
    while(getline(strstream, strLink))
    {
        explode(strLink, sslibev, ssrlibev, custom_port, local_port, &node);
        if(chkignore(&node, exclude_remarks, include_remarks))
        {
            writelog(LOG_TYPE_INFO, "Node  "+node.group+" - "+node.remarks+"  has been ignored and will not be tested.");
            continue;
        }
        else
            writelog(LOG_TYPE_INFO, "Node  "+node.group+" - "+node.remarks+"  has been added.");
        node.id = index;
        nodes->push_back(node);
        index++;
        //nodes->insert(nodes->end(), node);
    }

}
