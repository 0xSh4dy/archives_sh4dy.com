---
layout: page
title: Some AWS Commands
css: "/assets/css/modeOne.css"
readtime: true
comments: true
tags: [aws,cloud,awscli]
---

## STS and IAM

- Get details about the IAM user whose credentials are used to call the operation. In the output, view the ARN field to find out the username.
```
aws --profile profile_name sts get-caller-identity
```

- assume-role: Returns a set of temporary security credentials that you can use to access Amazon Web Services resources that you might not normally have access to. 
```
aws --profile profile_name sts assume-role --role-arn role_arn --role-session-name some_name
```
After that, set the returned information in the aws config file. For example,
```
[some_name]
aws_access_key_Id = your_access_key
aws_secret_access_key = your_secret_key
aws_session_token = some_session_token
region = some_region
```
This will allow us to use `some_name` as a temporary user with all the permissions assigned to the particular role.

- List the policies embedded in a specific IAM user
```
aws --profile profile_name iam list-user-policies --user-name user_name
```

- List roles
```
aws --profile profile_name iam list-roles
```
- List policies embedded in a specific IAM role
```
aws --profile profile_name iam list-role-policies --role-name role_name
```

- Retrieve the specified inline policy document that is embedded in the specified IAM user.
```
aws --profile profile_name iam get-user-policy --user-name user_name --policy-name policy_name

Similarly, we can use get-role-policy
```

- Retrieve information about a specific managed policy
```
aws --profile profile_name iam get-policy --policy-arn policy_arn
```
- Retrieve information about some version of a managed policy
```
aws --profile profile_name iam get-policy-version --policy-arn policy_arn --version-id version_num

version_num can be v1,v2,v3,etc.
```
- version_num in the above example can be found using
```
aws --profile profile_name iam list-policy-versions --policy-arn policy_arn
```
- List instance profiles that have the specified path prefix
```
aws --profile profile_name iam list-instance-profiles
```

## Lambda

- List all functions
```
aws --profile profile_name lambda list-functions
```

- Get information about a function
```
aws --profile profile_name lambda get-function --function-name function_name
```

- Create a new lambda function
```
aws --profile profile_name lambda create-function --function-name exploit_func --runtime python3.9 --role some_role --handler exploit.lambda_handler --zip-file fileb://exploit.zip --region some_region
```
In this case, `exploit.zip` should contain a file exploit.py with a function `lambda_handler`.


- Invoke a lambda function with arguments
```
aws --profile some_profile lambda invoke --function-name function_name --payload file://./payload.json out.txt
```
payload.json is the file storing the function arguments (in this case the expected function arguments were in JSON format) and out.txt stores the data returned by the Lambda function.

## EC2
- List EC2 instances
```
aws --profile profile_name ec2 describe-instances --region some_region
```
- Enumerate security groups
```
aws --profile profile_name ec2 describe-security-groups --region some_region
```
- List subnets
```
aws ec2 describe-subnets --profile profile_name --region us-east-1
```
- Create a new SSH keypair
```
aws --profile profile_name ec2 create-key-pair --key-name new-ec2-key --query 'KeyMaterial' --output text > new-ec2-key.pem --region some_region
```
- Launch a new EC2 instance
```
aws --profile profile_name ec2 run-instances --image-id ami-0a313d6098716f372 --instance-type t2.micro --iam-instance-profile Arn=some_arn --key-name new-ec2-key --subnet-id subnet_id --security-group-ids sg_id --region us-east-1
```

- Remove role from instance profile
```
aws iam remove-role-from-instance-profile --instance-profile-name instance_profile --role-name some_role --profile kerrigan
```
To add another role to an instance profile, you may run `add-role-to-instance-profile`

## Secrets Manager
```
aws --profile profile_name list-secrets --region some_region
```

References
<br>
https://docs.aws.amazon.com/