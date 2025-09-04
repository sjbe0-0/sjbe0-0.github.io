# 新版本安装
安装一些必要的依赖包：

```shell
sudo yum install -y yum-utils device-mapper-persistent-data lvm2
```

添加软件源信息：
```shell
# docker 官方源
sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
# 阿里云源
sudo yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
```
可选：启用edge和test存储库。这些存储库包含在上面的dockers.repo文件中，但默认情况下处于禁用状态。可以将他们与稳定存储库一起启用。
```shell
$ sudo yum-config-manager --enable docker-ce-edge
$ sudo yum-config-manager --enable docker-ce-test
```
可以通过使用--disable标志运行yum-config-manager命令来禁用边缘或测试存储库。要重新启用它，请使用--enable标志。以下命令禁用edge存储库：
```shell
$ sudo yum-config-manager --disable docker-ce-edge
$ sudo yum-config-manager --disable docker-ce-test
```
安装Docker-ce：
```shell
# 安装前可以先更新 yum 缓存：
sudo yum makecache fast
# 安装 Docker-ce
sudo yum install docker-ce
```
如果你想安装特定的docker-ce版本，先列出repo中可用版本，然后选择安装：
```shell
$ yum list docker-ce --showduplicates | sort -r
# docker-ce.x86_64       18.06.1.ce-3.el7              docker-ce-stable
# docker-ce.x86_64       18.06.1.ce-3.el7              @docker-ce-stable
# docker-ce.x86_64       18.06.0.ce-3.el7              docker-ce-stable
# docker-ce.x86_64       18.03.1.ce-1.el7.centos       docker-ce-stable
# docker-ce.x86_64       18.03.0.ce-1.el7.centos       docker-ce-stable
# docker-ce.x86_64       17.12.1.ce-1.el7.centos       docker-ce-stable
# 选择版本安装
$ sudo yum install docker-ce-<VERSION STRING>

# 选择安装 docker-ce-18.06.1.ce
$ sudo yum install docker-ce-18.06.1.ce
```
启动Docker后台服务：
```shell
sudo systemctl start docker
```
通过运行hello-world镜像，验证是否正确安装了docker。
```shell
sudo docker run hello-world
```
# 旧版本安装

```shell
yum install docker        # CentOS 中安装
apt-get install docker-ce # Ubuntu 中安装
pacman -S docker          # Arch 中安装
emerge --ask docker       # Gentoo 中安装

#=====================
docker version      # 通过查看版本，检查安装是否成功
# Client:
#  Version:         1.12.6
#  API version:     1.24
#  Package version: docker-1.12.6-55.gitc4618fb.el7.centos.x86_64
#  Go version:      go1.8.3
#  Git commit:      c4618fb/1.12.6
#  Built:           Thu Sep 21 22:33:52 2017
#  OS/Arch:         linux/amd64
# 
# Server:
#  Version:         1.12.6
#  API version:     1.24
#  Package version: docker-1.12.6-55.gitc4618fb.el7.centos.x86_64
#  Go version:      go1.8.3
#  Git commit:      c4618fb/1.12.6
#  Built:           Thu Sep 21 22:33:52 2017
#  OS/Arch:         linux/amd64
```
# 命令介绍
```shell
$ docker --help

管理命令:
  container   管理容器
  image       管理镜像
  network     管理网络
命令：
  attach      介入到一个正在运行的容器
  build       根据 Dockerfile 构建一个镜像
  commit      根据容器的更改创建一个新的镜像
  cp          在本地文件系统与容器中复制 文件/文件夹
  create      创建一个新容器
  exec        在容器中执行一条命令
  images      列出镜像
  kill        杀死一个或多个正在运行的容器    
  logs        取得容器的日志
  pause       暂停一个或多个容器的所有进程
  ps          列出所有容器
  pull        拉取一个镜像或仓库到 registry
  push        推送一个镜像或仓库到 registry
  rename      重命名一个容器
  restart     重新启动一个或多个容器
  rm          删除一个或多个容器
  rmi         删除一个或多个镜像
  run         在一个新的容器中执行一条命令
  search      在 Docker Hub 中搜索镜像
  start       启动一个或多个已经停止运行的容器
  stats       显示一个容器的实时资源占用
  stop        停止一个或多个正在运行的容器
  tag         为镜像创建一个新的标签
  top         显示一个容器内的所有进程
  unpause     恢复一个或多个容器内所有被暂停的进程    
```

# 服务管理
```shell
service docker start       # 启动 docker 服务，守护进程
service docker stop        # 停止 docker 服务
service docker status      # 查看 docker 服务状态
chkconfig docker on        # 设置为开机启动
```

# 镜像管理
镜像可以看做我们平时装系统的镜像，里面就是一个运行环境。

```shell
docker pull centos:latest  # 从docker.io中下载centos镜像到本地
docker images              # 查看已下载的镜像
docker rmi [image_id]      # 删除镜像，指定镜像id

# 删除所有镜像
# none 默认为 docker.io
docker rmi $(docker images | grep none | awk '{print $3}' | sort -r)

# 连接进行进入命令行模式，exit命令退出。
docker run -t -i nginx:latest /bin/bash
```

## 下载镜像本地安装镜像
由于国区已经无法访问，可以将镜像打包，直接安装镜像压缩文件
```shell
$ docker pull gitlab/gitlab-ce:17.2.0-ce.0 # 下载镜像
$ docker pull --platform linux/amd64 gitlab/gitlab-ce:17.2.0-ce.0
$ docker pull --platform linux/amd64 portainer/portainer-ce:2.20.3-alpine
$ docker pull --platform linux/amd64 portainer/agent:2.20.3-alpine
```
## 保存 Docker 镜像到本地文件
```shell
$ docker save -o [output-file.tar] [image-name]
$ docker save -o gitlab-ce.0-17.2.0.tar gitlab/gitlab-ce:17.2.0-ce.0
$ docker save -o portainer-agent-2.20.3-alpine.tar portainer/agent:2.20.3-alpine
$ docker save -o portainer-ce-2.20.3-alpine.tar portainer/portainer-ce:2.20.3-alpine
```
## 将镜像文件发送到服务器
```shell
$ scp [output-file.tar] [user]@[server-ip]:[path]
$ scp gitlab-ce-17.0.1.tar root@110.55.8.163:/home/docker-images
```
## 服务器上加载 Docker 镜像
```shell
$ docker load -i /home/docker-images/gitlab-ce-17.0.1.tar # 加载 Docker 镜像
$ docker images # 验证镜像是否加载成功
```

## 通过容器创建镜像
我们可以通过一下两种方式对镜像进行更改
1. 从已经创建的容器中更新镜像，并且提交这个镜像
2. 通过 Dockerfile 创建一个新的镜像

下面通过以存在的容器创建一个新的镜像
```shell
$ docker commit -m="First Docker" -a="wcjiang" a6boa6cfdasf wcjiang/nginx：v1.2.1"
```
上面命令参数说明：
- -m:提交的描述信息
- -a:作者信息
- a6boa6cfdasf:容器ID，不是镜像id
- wcjiang/nginx：v1.2.1:镜像名称和版本号

## 通过Dockerfile创建镜像
假设创建一个node.js镜像，首先在node.js项目根目录创建文件。
```shell
touch Dockerfile .dockerignore
```
Dockerfile内容如下：
```shell
FROM node:12.16.1-alpine
COPY . /app
WORKDIR /app
RUN npm install --registry=https://registry.npm.taobao.org
EXPOSE 3000
```

- FROME node:12.16.1-alpine:该image文件继承官方的node image，冒号表示标签，这里标签是8.4，即8.4版本的node。
- COPY . /app:将当前目录下的所有文件（除了.dockerignore排除的路径），都拷贝进image文件/app目录。
- WORKDIR /app:指定接下来的工作路径为/app。
- RUN npm install --registry=https://registry.npm.taobao.org:在image文件/app目录下，运行npm install命令，使用淘宝镜像源。
- EXPOSE 3000:将容器的3000端口暴露出来。允许外部连接这个接口。

有了Dockerfile文件以后，就可以使用docker image build命令创建image文件了。
```shell
docker image build -t koa-demo
或者
docker image build -t koa-demo：0.0.1
```

上面命令，将刚创建的koa-demo镜像跑起来，命令的--rm参数，在容器终止运行后自动删除容器文件。


## 发布自己的镜像
1. 在Docker注册账户，发布的镜像都在这个页面里展示
2. 将上面做的及各项nginx，起个新的名字nginx-test
```shell
docker tag wcjiang/nginx:v1.2.1 wcjiang/nginx-test:lastest
```
3. 登录docker
```shell
docker login
```
4. 上传nginx-test镜像
```shell
docker push wcjiang/nginx-test:lastest
```
5. 验证上传成功
```shell
docker images
```

## 镜像中安装软件
通常情况下，使用dcker官方镜像，如mysql镜像，默认情况下镜像中什么软件也没有，通过下面的命令安装你所需要的软件：
```shell
# 第一次需要运行这个命令，确保源的索引是最新的
# 同步 /etc/apt/sources.list 和 /etc/apt/sources.list.d 中列出的源的索引
apt-get update
# 做过上面更新同步之后，可以运行下面的命令了
apt-get install vim
```
如果你安装了Centos或者Ubuntu系统可以进入系统安装相关软件
```shell
# 进入到centos7镜像系统
docker run -i -t centos:7 /bin/bash
yum update
yum install vim
```
# 容器管理
容器就像一个类的实例
```shell
# 列出本机正在运行的容器
docker container ls
# 列出本机所有容器，包括终止运行的容器
docker container ls --all
docker start [containerID/Names] # 启动容器
docker stop [containerID/Names]  # 停止容器
docker rm [containerID/Names]    # 删除容器
docker logs [containerID/Names]  # 查看日志
docker exec -it [containerID/Names] /bin/bash  # 进入容器

# 从正在运行的 Docker 容器里面，将文件拷贝到本机，注意后面有个【点】拷贝到当前目录
docker container cp [containID]:[/path/to/file] .

docker run centos echo "hello world"  # 在docker容器中运行hello world!
docker run centos yum install -y wget # 在docker容器中，安装wget软件
docker ps                             # 列出包括未运行的容器
docker ps -a                          # 查看所有容器(包括正在运行和已停止的)
docker logs my-nginx                  # 查看 my-nginx 容器日志

docker run -i -t centos /bin/bash     # 启动一个容器
docker inspect centos                 # 检查运行中的镜像
docker commit 8bd centos              # 保存对容器的修改
docker commit -m "n changed" my-nginx my-nginx-image # 使用已经存在的容器创建一个镜像
docker inspect -f {{.State.Pid}} 44fc0f0582d9        # 获取id为 44fc0f0582d9 的PID进程编号
# 下载指定版本容器镜像
docker pull gitlab/gitlab-ce:11.2.3-ce.0
```
# 容器服务管理
```shell
docker run -itd --name my-nginx2 nginx  # 通过nginx镜像，【创建】容器名为 my-nginx2 的容器
docker start my-nginx --restart=always  # 【启动策略】一个已经存在的容器启动添加策略
    # no - 容器不重启
    # on-failure - 容器推出状态非0时重启
    # always - 始终重启
docker start my-nginx             # 【启动】一个已经存在的容器
docker restart my-nginx           # 【重启】容器
docker stop my-nginx              # 【停止运行】一个容器
docker kill my-nginx              # 【杀死】一个运行中的容器
docker rename my-nginx new-nginx  # 【重命名】容器
docker rm new-nginx               # 【删除】容器
```
# 进入容器
1. 创建一个守护状态的Docker容器
docker run -itd my-nginx /bin/bash
2. 使用docker ps查看该容器信息
docker ps
3. 使用dockers exec命令进入一个已经在运行的容器
docker exec -it 6bd0496da64f /bin/bash

通常有下面几种方式进入Docker容器，推荐使用exec，使用attach一直进入失败。
- 使用docker attach
- 使用SSH为什么不需要再Docker容器中运行sshd
- 使用nsenter进入Docker容器，nsenter官方仓库
- 使用docker exec，在1.3.*之后提供了一个新的命令exec用于进入容器

# 文件拷贝
从主机复制到容器 sudo docker cp host_path
containerID：container_path从容器复制到主机 sudo docker cp
containerID：container_path host_path
