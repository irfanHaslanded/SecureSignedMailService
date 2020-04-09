# SecureSignedMailService
A simple application which encrypts and signs messages for each user

```bash
export LD_LIBRARY_PATH=/app/vbuild/RHEL6-x86_64/gcc/6.4.0/lib:/app/vbuild/RHEL6-x86_64/gcc/6.4.0/lib64
```

## Logging

Program writes logs in order to be able to demonstrate secure data transfer.
Logs are written to the standard error to be able to easily separate from
application's printouts. In order to redirect logs into a file start app as
follows.

```bash
ssms 2>> log.txt
```

In a different console, the logs can be followed in real-time:

```bash
tail -f log.txt
```

## Password storage

![Salting](https://www.okta.com/sites/default/files/styles/wysiwyg_full_width/public/media/storing_salted_passwords.png)

![Password hash](https://www.php.net/manual/en/images/2a34c7f2e658f6ae74f3869f2aa5886f-crypt-text-rendered.svg)

## Encryption/decryption flow

The image below is taken from the [Wikipedia page of PGP (Pretty Good Privacy)](https://en.wikipedia.org/wiki/Pretty_Good_Privacy)

![Encryption flow](https://upload.wikimedia.org/wikipedia/commons/4/4d/PGP_diagram.svg)

