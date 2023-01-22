---
layout: page
title: AWS Services
css: "/assets/css/modeOne.css"
readtime: true
comments: true
tags: [aws,cloud,awscli]
---

<hr>

## IAM (Identity and Access Management)
AWS IAM is a web service that helps you securely control access to AWS resources. With IAM, you can centrally manage permissions that control which AWS resources users can access in what ways.

Four main components of IAM are users, groups, roles and policies.

#### Users
Am IAM user is an entity within an AWS account that has specific permissions for a single person or application. 

#### Groups
A group is an entity that specifies a collection of IAM users. All the users present in a group inherit the permissions provided to a group.

#### Roles
A role is an entity that has specific permissions. It is similar to an IAM user, but is not associated with any specific person. For example, a role may provide the permissions to an EC2 instance to access S3 buckets.

#### Policies
To assign permissions to a user, group, role,etc., you create a policy, which is a document that explicitly lists permissions. For example, we may create a policy that allows to perform some actions and access all the resources or some specifc resources of an EC2 instance. There are different types of policies such as `Identity-based policies`, `Resource-based policies`, `Permission boundaries`, `Organization SCPs`, `Access Control Lists`, and `Session policies`

- `Identity-Based Policies` are JSON documents that control what actions an identity (users, group of users or roles) can perfom, on which resources and under what conditions. Identity-based policies can  be further classified as:

`Managed Policies`: Standalone identity based policies that can be attached to multiple users, groups, and roles in an AWS account. `AWS managed policies` are created and managed by AWS whereas `Customer managed policies` are policies that you create and manage in your AWS account.

<br>

`Inline policies`: Policies that you add directly to a single user, group or role. Inline policies maintain a strict one-to-one relationship between a policy and an identity. They are deleted when you delete the identity.

- `Resource-based policies` are policy documents that can be attached to a resource such as a S3 bucket. These policies grant specific permissions to perform specific actions on a resource and under what conditions. The IAM service supports only one resource based policy which is known as role trust policy.

#### Access Keys
Access keys are long-term credentials for an IAM user or the AWS account root user. Access keys can be used to enable programmatic access to an AWS account.

The following command returns details about the IAM user or role whose credentials are used to call the operation.

The following command can be used to find the username, provided access key and secret are available.
```
aws --profile profile_name iam get_user
```
The following command can be used to get available policies
```
aws --profile profile_name iam list-attached-user-policies --user-name userName
```
```
aws --profile profile_name sts get-caller-identity
```
<hr>

### Lambda
AWS Lambda is a serverless, event driven computing service that lets you run code for any type of application or backend service without provisoning or managing servers. AWS lambda is widely used for processing objects stored in S3. With lambda, you don't need to worry about the server, scaling and cost. Charges are applied only for the amount of time the code is executed.
Lambda contains layers which provide a convenient way to package libraries and other dependencies that can be used within a lambda function. 
```
aws --profile profile_name --region your_region lambda list-functions
aws --profile profile_name --region your_region lambda get-policy --function-name function_name
```
In case the lambda function is called via an API gateway, we may find the API id.
```
aws --profile profile_name --region your_region apigateway get-stages --rest-api-id "api_id"
```
<hr>

## S3 (Simple Storage Service)
Amazon S3 is a service offered by AWS that provides object storage through a web based interface. S3 can also be used programatically. It can be used to store everything, for example media files, code, spreadsheets,etc. S3 is highly scalable because the data isn't stored on a single device but split across multiple data centres setup by Amazon. The data is also stored across multiple replicas which eliminates the chances of downtime. The data is stored in buckets. Public access to S3 objects is disabled by default.
A bucket is a container for objects stored in AWS S3. Buckets have a unique name. Within a bucket, multiple subfolders and files can be created. An object is just some content (file) that is stored in a bucket. Objects can be accessed using url as well as programatically (using boto3 SDK). Buckets and objects can be classified into various storage classes (for example, glacier, intelligent, infrequent access and standard).

#### Standard
It offers high durability, availability and performance object storage for frequently accessed data. It has low latency and delivers high throughput, so it can be used for a variety of cases.

#### Intelligent
It automatically reduces storage costs on a granular level by automatically moving data to the most cost-effective access tier based on access frequency, without performace impact, retrieval fees or operational overhead.

#### Infrequent Access
It is for the data that is accessed less frequently, but requires rapid access whenever needed. It offers high durability, throughput and low latency.Transition from standard to infrequent access can be done immediately.

#### Glacier
It is another storage class for infrequently accessed data. Glacier storage classes are optimized for data archiving, and lowest cost cloud storage. Transition from standard to glacier takes around 1 day.

#### Lifecycle Rules
Lifecycle configuration is a set of rules that define actions that Amazon S3 applies to a group of objects. There are two types of actions namely transition actions and expiration actions.
<br>
`Transition actions` define when objects are transferred from one storage class to another storage class. There are costs associated with lifecycle transition requests. `Expiration actions` define when objects expire. Amazon S3 deletes expired objects automatically.


### S3 Security
- Public access is disabled by default.
- S3 applies server side encryption with Amazon S3 managed keys as the base level of encryption for every bucket.
- Access to S3 buckets can be managed using AWS IAM.
- AWS supports access logs, action based logs and alarms on buckets using AWS Cloud Watch.

### Other Uses of S3

- S3 can be directly integrated with Amazon Athena to provide analytics like services.
- S3 can be used with AWS Lambda to directly process the data stored in it.
- S3 can also be used for hosting static websites. For more information, [click here](https://www.youtube.com/watch?v=fm6FPQMZ_WI)

### Enumerating S3 buckets
We can use AWS CLI to dump the files present in a S3 bucket
```
aws s3 ls s3://bucket_name/ --no-sign-request --region region_name
```
Sometimes people make an error of opening permissions for `Any authenticated AWS user`, thinking that these users would be the ones that have access to the same account, when in fact it means anyone who has an AWS account.

The `profile` mentioned above can be set using `aws configure --profile profile_name`.
To dump the files present in a bucket to a local directory, run the following command.
```
aws s3 sync s3://bucket /path_to_directory
```
#### Printing the names of all the buckets in an authenticated account
There are two ways to do this: through AWS CLI and through boto3.
```
AWS CLI
aws --profile profile-name s3 ls
```
```py
Using Python
import boto3

aws_access_key_id = "access_key_id"
aws_secret_access_key = "secret_access_key"

session = boto3.Session(aws_access_key_id,aws_secret_access_key)
s3 = session.resource('s3')
buckets_iter = s3.buckets.pages()
for bucket in buckets_iter:
	print(bucket)
```

<hr>

### Amazon Elastic Block Store
Amazon Elastic Block Store provides raw block-level storage that can be attached to EC2 instances and is used by the Amazon Relational Database Service (RDS).

<hr>

### EC2 (Elastic Compute Cloud)
EC2 provides scalable computing capacity in the AWS cloud. EC2 can be used to launch multiple virtual machines on the cloud. When an administrator creates an EC2 instance, AWS typically creates an Elastic Block Store (EBS) volume that is to be used by the instance.

Listing EC2 instances using AWS CLI
```
aws --profile profile_name ec2 describe-instances  > instances.txt
```
#### Instance Profile
Just like an IAM user represents a person, an instance profile represents EC2 instances. An instance profile is a container for an IAM role that you can use to pass role information to an EC2 instance when the instance starts.

For roles related to EC2, if you're not allowed to do `list-role-policies`, just attach it to an EC2 instance. It can be done if you're allowed to create/modify instance profiles. To create a new EC2 instance using AWS CLI, we need the following-

1. A subnet to start the EC2 instance in.
2. A security group that allows SSH access to the EC2 instance.
3. An SSH keypair to SSH into the EC2 instance.

Install AWS CLI on the EC2 instance and then start enumerating roles and policies.

#### EC2 snapshots
A snapshot is a copy of the storage blocks that have been modified since the previous snapshot was created. The first snapshot contains a full copy of the volume. Subsequent snapshots contain only the modified storage blocks.

```
aws --profile profile_name ec2 describe-snapshots --owner-id owner_id

owner_id is a 12 digit number.
(Remove the owner_id field to view all accessible snapshots)
```
Get all snapshots from a volume
```
aws --profile profile_name ec2 describe-snapshots --filter "Name=volume-id,Values=volume_id" --region region
```

#### Mounting an EBS volume on an EC2 instance
Create a volume using Elastic Block Store, set the snapshot id if you already have one and attach it to the EC2 instance. Please note that the availability zones of the snapshot and the instance must be the same. After that, get into the EC2 instance using ssh. Run `lsblk` to see different partitions and then mount it.
```
mount /dev/xvdf1 /mnt
```
Snapshots are normally restricted to your own account, so a possible attack would be an attacker getting access to an AWS key that allows them to start/stop and do other things with EC2's and then uses that to snapshot an EC2 and spin up an EC2 with that volume in your environment to get access to it. Like all backups, you need to be cautious about protecting them.

<hr>

### The Magic IP: 169.254.169.254
Most EC2 instances have access to the metadata service at `169.254.169.254`. Not only AWS, but other cloud providers such as Google Cloud, Microsoft Azure, Digital Ocean,etc. use this magic IP to access the metadata service. To verify this, you can just ssh into an EC2 instance and run `curl http://169.254.169.254`. This contains useful information about the EC2 instance such as ip address, security groups, etc. On EC2 instances that have an IAM role attached, the metadata service will also contain the IAM credentials to authenticate as this role. This can be exploited using SSRF.

Note: Google Cloud adds additional constraints on the requests, such as requiring it to use `Metadata-Flavor: Google` as an HTTP header and refusing requests with an `X-Forwarded-For` header. AWS has recently created a new IMDSv2 that requires special headers, a challenge and response, and other protections, but many AWS accounts may not have enforced it.


### Getting policies
```
aws --profile profile_name iam get-user
aws --profile profile_name iam list-attached-user-policies --user-name user_name
aws --profile profile_name iam get-policy  --policy-arn policy_arn
aws --profile profile_name iam get-policy-version  --policy-arn policy_arn --version-id version_id
```

<hr>

### CloudTrail
AWS CloudTrail monitors and records account activity across your AWS infrastructure, giving you control over storage, analysis and remediation actions. Using CloudTrail, you can find who or what took what action, what resources were used, the event time, etc. CloudTrial is enabled in AWS accounts by default. Each activity is recorded in a CloudTrail event. These events can be viewed from the `Event History` tab. Event history allows you to view, search and download the past 90 days activity in your AWS account. In addition, users may create a trail to archive, analyze and respond to changes in AWS resources. 

#### CloudTrail Events
An event is nothing but a record of activity in an AWS account. This activity can be any action taken by a user, role, or service that is monitorable by CloudTrail. There are three types of events that can be logged in AWS CloudTrail namely management events, data events and insight events. By default, trails log management events, but not data and insight events.
<br>
`Management events` , or `control plane operations` provide information about management operations that are performed on resources in an AWS account. It includes operation such as configuring security, registering devices (such as an EC2 instance), configuring rules for routing and setting up logging. It can also log non API events such as when a user logs in.

`Data events`, or `data plane operations` provide information about the resource operations performed on or in a resource. It stores information about S3 object-level API activity(for example, operations on buckets), lambda function execution activity, activity on DynamoDB, EBS,etc.

`Insights events` capture unusual API call rate or error rate activity. Insight events provide relevant information such as error code, incident time, statistics, etc. Insight events are logged only when AWS detects weird API usage or request rates that differ significantly from the usual activity. 

#### CloudTrail Event History
The CloudTrail Event History stores a log of all the events that occured in the past 90 days. The logs can be downloaded for further analysis.

#### Trails
A `trail` is a configuration that enables delivery of events to an Amazon S3 bucket. Trails are of two types, for example a trail that applies to all AWS regions and a trail that applies to one AWS region. Apart from that, there are organization trails that log all events for either one region or all regions in a particular organization.

#### CloudWatch
AWS CloudWatch is a web service that collects and tracks metrics to monitor AWS resources and applications that you run on AWS. It is used for real time monitoring of AWS resources and applications. 
It also detects irregular behaviour in an AWS account. 

`CloudWatch events`: An AWS service that delivers a near real-time stream of system events that describes changes in AWS resources. In CloudWatch events, you can create rules that trigger on any event recorded by CloudTrail.


It is a good practice to have a separate security account that contains the CloudTrail logs from all other AWS accounts and also has some sort of access into other accounts to check up on things. `jq` is a powerful command-line JSON processor that can be used to analyze CloudTrail logs. Download the CloudTrail logs as well as jq and then run the following command on the current folder (containing CloudTrail logs) to get a beautified view of logs.
```
find . -type f -exec cat {} \; | jq .
```
To filter out the key `eventName` from an array `Records`, run the command
```
find . -type f -exec cat {} \; | jq '.Records[]|.eventName'
```
To add one more field let's say `eventTime` in output, we can do:
```
find . -type f -exec cat {} \; | jq '.Records[]|[.eventTime,.eventName]'
```
To convert the array `[.eventTime,.eventName]` into a string where various fields ar separated by a `\t`, add the field `@tsv`
```
find . -type f -exec cat {} \; | jq -cr '.Records[]|[.eventTime,.eventName]|@tsv'
```
To sort the output, run the following command
```
find . -type f -exec cat {} \; | jq -cr '.Records[]|[.eventTime,.eventName]|@tsv' | sort
```
The -cr prints the data in a row, and the |@tsv makes this tab separated. 
To select a specific record, run the following command with jq
```
jq '.Records[]|select(.eventName=="ListBuckets")'
```

<hr>

### AWS Secrets Manager
AWS Secrets Manager is a secrets management service that helps you protect access to your applications, services, and IT resources. This service enables you to easily rotate, manage, and retrieve database credentials, API keys, and other secrets throughout their lifecycle.

References
<br>
https://docs.aws.amazon.com/
http://flaws2.cloud
<br>
https://docs.aws.amazon.com/awscloudtrail/latest/userguide/how-cloudtrail-works.html