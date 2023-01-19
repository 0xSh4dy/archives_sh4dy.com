### Container Orchestration
Container orchestration is the automation of much of the operational effort required to run containerized workloads and services. It includes a wide range of things such as container's lifecycle, deployment, scaling, networking, load balancing, etc.

### Kubernetes
It is an open source container orchestration framework, originally developed by Google. Some advantages of using kubernetes are high availability (no downtime), scalability (high performance), disaster recovery( backup and restore).

### Pod
A pod is the smallest execution unit in Kubernetes. It is a collection of one or more containers and provides a new abstraction layer over containers. Pods are usually used to run a single application. Each pod gets its own internal IP address. Pods can communicate with each other using this IP address. 

### Service
When a pod dies (maybe due to lack of resources or some crash, etc.), a new pod is created and a new IP address is assigned to it. It might be inconvenient to use a new IP address in some cases. A service is an abstraction which defines a logical set of pods and a policy by which to access them. Services can be assigned a unique IP address. Lifecycle of a service and a pod are not connected so even if the pod dies, the service and its IP will stay intact. A service can also be used as a load balancer.

### Ingress
An ingress is an API object that provides routing rules to manage external users' access to the services in a k8s cluster. It forwards requests to services.

### ConfigMap
An API object used to store non-confidential data in key-value pairs.

### Secret
A secret is an object that can be used to store confidential data such as API keys, passwords,etc. Secrets are similar to ConfigMaps but are specifically intended to hold sensitive data. The values for all the keys in the data field must be base64 encoded.

### Volumes
A volume represents a directory with data that is accessible across multiple containers in a pod. The container data in a pod is lost when a container crashes or restarts, but when you use a volume, the new container can pickup the data at the state before the container crashes.

### Deployment
In case a pod goes down or the application needs to be restarted, external users might face trouble. Creating a replication of the application solves the problem of downtime. Deployments represent a set of multiple, identical pods with no unique identities. A deployment runs multiple replicas of an application and automatically replaces any instances that fail or become unresponsive.

### ReplicaSet
A ReplicaSet ensures that a specified number of pod replicas are running at any given time. They are managed using deployments.



<hr>

## Kubernetes Architecture

### Node
A pod always runs on a node. A node is a worker machine in kubernetes and may be either a virtual machine or a physical machine. 3 processes namely container runtime, kubelet and kube proxy must be installed on each node.

### Control Plane
It manages the worker nodes and pods in the cluster. This layer exposes the API and interfaces to define, deploy and manage the lifecycle of containers. This layer is made up of different components such as etcd, apiserver, scheduler, controller manager, cloud controller manager, etc.

### Kubelet 
It is the primary `node agent` that runs on a node. It can register the node with the apiserver using one of the hostname, a flag to override the hostname, or specific logic for a cloud provider.

### Cluster
A cluster is a set of nodes for running containerized applications.

### Master Node
The master node is responsible for cluster management and for providing the API that is used for managing resource within the Kubernetes cluster. 4 master processes run on every master node.

1. `Apiserver`: The apiserver validates and configures data for the api objects which includes pods, services, replication controller and others. It validates requests and sends them to other processes.

2. `Scheduler`: A scheduler watches for newly created pods that have no nodes assigned. It is responsible for finding the best node for a pod to run. The kubelet receives request from the scheduler and executes the request on the node.

3. `Controller Manager`: It manages controllers in k8s. Controllers are control loops(a control loop is non-terminating loop that regulates the state of a system) that continously watch the state of the cluster, then make or request changes where needed.

4. `etcd`: It is a key-value store used as Kubernetes' backing store for all cluster data. Every change in the cluster (such as creation or deletion of pods) are saved or updated in the etcd store. `etcd` does not store the application data. The data stored in etcd is important for communication between master nodes and worker nodes.

<hr>

### Minikube
Minikube is local kubernetes that runs master and worker processes on the same machine.

### kubectl
A command line tool that can be used to run commands against kubernetes clusters.

### minikube and kubectl commands
```
alias kubectl="minikube kubectl --"
```
```
Command                
minikube start      -> Start a cluster
minikube status     -> Check status of various master processes

kubectl exec POD [-c CONTAINER] -- COMMAND [args...]
kubectl exec -it nginx-depl-c88549479-tkw2k bash    -> Get a shell in the container
```
```
kubectl get nodes   
kubectl create deployment deployment_name --image=image_name
kubectl delete deployment deployment_name
kubectl get deployment
kubectl get replicaset
kubectl get pod
kubectl edit deployment deployment_name
kubectl logs pod_name
kubectl apply -f config_filename
kubectl describe service service_name
kubectl get pod -o wide
kubectl describe pod pod_name
kubectl get all
kubectl get namespaces
```
<hr>

### K8s config file
Consider the following config for nginx deployment and service respectively.


```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx-deployment
  labels:
    app: nginx
spec:
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:1.16
        resources:
          limits:
            memory: "128Mi"
            cpu: "500m"
        ports:
        - containerPort: 80

```

```yaml
apiVersion: v1
kind: Service
metadata:
  name: nginx-service
spec:
  selector:
    app: nginx
  ports:
  - port: 80
    targetPort: 8080
```
Each configuration file has three parts: metadata, specification and status. Status is automatically managed by kubernetes and is updated frequently, by kubernetes. The data to be added or updated comes from etcd.

`metadata`: It describes the deployment itself. It gives a label for that actual deployment.
<br>
`template`: It is actually a pod template (a configuration for pods). It describes a pod that is launched. The `label` field present in template is for pod, not for deployment.
<br>
`matchLabels`: It tells the resource such as deployment to match the pod according to the label. Only Job, ReplicaSet, DaemonSet and Deployment support matchLabels.

<hr>

### port vs targetPort vs nodePort
port: Exposes the service on the specified port internally within the cluster. The service becomes visible on this port, and will send the requests made to this port to the pods selected by the service.
<br>
targetPort: The port on the pod that the request gets sent to.
<br>
nodePort: Exposes the service on each node's IP at a static port.

### Namespaces
Namespaces provide a mechanism for isolating groups of resources within a same cluster. By default, kubernetes provides 4 namespaces. 

1.`kube-system`: The namespace for objects created by the Kubernetes system. This isn't meant for regular use and shouldn't be messed up with.

2.`kube-node-lease`: This holds lease objects associated with each node. A lease provides a mechanism to lock shared resources and coordinate activity between nodes.

3. `kube-public`: It stores publicly accessible data for example a configmap, that stores cluster information
```
kubectl cluster-info
```
4. `default`: Kubernetes includes this namespace so that you can start using your new cluster without first creating a namespace.

### Why to use namespaces?

1. A complex application might have various ConfigMaps, Secrets, replicas,services etc. This will make the default namespace a little bit messy. Using namespaces solves this problem. For example, you may create a database namespace that stores all the resources related to your databse, a monitoring namespace, a namespace for nginx-ingress, etc. Thus, namespaces are nothing but a way to group resources in a cluster.

2. Consider two teams using the same cluster. One team might deploy an application with a certain configuration. The other team's deployment may have same name but some different configuration which would override the work done by the first team. Namespaces can be used to avoid this problem. A team can be restricted to work only on their own namespace.

3. Namespaces can also be used for sharing hardware resources, for example a fixed amount of RAM and CPU.

### Characteristics of Namespaces
1. ConfigMaps and Secrets cannot be accessed from a different namespace, so each namespace needs to create its own ConfigMap / secrets. However, services can be shared.

2. Components such as node and volumes cannot be created in a namespace (they exist globally in the cluster). Run the following command to get a list of all such components:
```
kubectl api-resources --namespaced=false
```
Example:
<br>
To create a ConfigMap in `abcd` namespace, run the following command:
```
kubectl create namespace abcd
kubectl apply -f --namespace=abcd
```
To get information about that ConfigMap,
```
kubectl get configmap -n abcd
```

### kubens tool can be used for working with namespaces.

### Ingress
Ingress is an API object that provides routing rules to manage access to services within a kubernetes cluster. `IngressController` is a pod or a set of pods that runs on nodes in a k8s cluster and does evaluation and processing of ingress rules. 
In minikube, an IngressController can be installed using the following command:
```
minikube addons enable ingress
```
It acts as a reverse proxy and load balancer.It adds a layer of abstraction to traffic routing, accepting traffic from outside the Kubernetes platform and load balancing it to pods running inside the platform. An ingress with no rules sends all the traffic to the default backend.
```
kubectl describe ingress ingress_name
```

### Volumes
A volume represents a directory with data that is accessible across multiple containers in a pod. The container data in a pod is lost when a container crashes or restarts, but when you use a volume, the new container can pickup the data at the state before the container crashes. Ideal requirements for a storage are:
1. Storage that doesn't depend on the pod lifecycle.
2. Storage must be available on each node.
3. Storage must survive even if the cluster crashes.

#### emptyDir
emptyDir are volumes that get created empty when pods are created. Deleting a pod deletes all such volumes.

#### hostPath
A hostPath volume mounts a file or directory from the file system of the host node to a pod.

#### Persistent Volume
PV is a storage resource in a cluster that has been provisioned by an administrator or dynamically provisioned using storage classes. They are not namespaced i.e. they are available within the entire cluster. For applications to use the storage, some configuration needs to be done. Applications have to claim their volume storage using `Persistent Volume Claims`. A PVC claims a volume with certain capacity and some other characteristics. The claim finds a persistent volume in the cluster that satisfies the claim. Claims must exist in the same namespace as the pod using them. The volume is mounted into the pod and subsequently mounted into the container.
`ConfigMap` and `Secret` are local volumes but they are not created via PV and PVC

#### StorageClass
It provisions(creates) Persistent Volumes dynamically whenever PVC claims it.

### Stateful Set
Databases cannot be replicated via deployment because they have state i.e. the data stored in a database may change frequently. A StatefulSet can be used to manage pods whose state may change. StatefulSet can be used for replicating pods. However, deploying a StatefulSet is not easy. In StatefulSet, a new pod isn't create until the previous one is complete. Each pod in a StatefulSet gets its own DNS endpoint from the service (this feature is not present in deployment). The DNS endpoint looks as: ${pod name}.${governing service domain}. When pods restart, their IP will change but their name and DNS endpoint will remain the same (sticky identity).

#### Scaling Database Applications
Consider a mysql pod and its multiple replicas. Using multiple pods for read-write operations may cause race condition. Therefore, only one pod is allowed to perform both read-write applications. This pod is known as the master. Other pods (slaves) are only allowed to read data. Even though these pods store the same data, they do not use the same storage. Each of them has a replica of the storage. Newly created pods clone data from the previous pod. After cloning the data, the pod ensures continuous synchronization with other pods. There's a flaw in this setup. All the data will be lost if the master pod dies. To ensure data persistence, Persistent Volumes can be used.

#### Important differences b/w deployment and StatefulSet
In deployment, pods get random hashes whereas the StatefulSet pods get fixed ordered names which consists of the stateful set name and the ordinal (starts from 0 and increases). For example, mysql-0, mysql-1, mysql-2, etc. In StatefulSet, pod deletions are done in reverse order.

### Init Containers
Init containers are specialized containers that run before other containers are started in a pod.