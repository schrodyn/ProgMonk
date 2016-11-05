##Burp Suite Cheatsheet

## Running

start script:

```sh
#!/bin/sh
exec $JAVA_HOME/bin/java -jar -Xms512m -Xmx2G /usr/share/burpsuite/burpsuite.jar $@
```

  * **-Xms** -- startup and minimum heap size
  * **-Xmx** -- max heap size

## Font Config

To configure Java Font edit **fontconfig.properties** file which
is located in **jre/lib** directory. For example, on my system
it is located in the following folder

```
/usr/lib/jvm/java-7-openjdk/jre/lib
```

After this you should replace all DejaVu fonts (which are default)
on your own. For example, I've replaced all default fonts on
**FreeSans** and then added following lines in config to add 
**FreeSans** font.

```
filename.FreeSans=/usr/share/fonts/TTF/FreeSans.ttf
filename.FreeSans_Bold=/usr/share/fonts/TTF/FreeSansBold.ttf
```
To add **Anti-aliasing** of fonts use Java command-line option:

```
-Dawt.useSystemAAFontSettings=on
```

Finally, to do this system-wide, add the following line to 
`/etc/environment`:

```
_JAVA_OPTIONS='-Dawt.useSystemAAFontSettings=on'
```

You can use `_JAVA_OPTIONS` to pass default options to any JVM process 
started on your system.

## Proxy Configuration

### Mozilla Firefox

  * Go to **Preferences -> Advanced -> Network -> Settings**
  * Select **Manual proxy configuration**
  * HTTP proxy: **127.0.0.1** Port: **8080** 
  * Check **Use this proxy server for all protocols**
  * Remove everything from field **No Proxy for:**

**Reminder:** After applying this settings **https** interception 
will not work. To setup it see **SSL certificate setup** section 
below.

### Chromium (Chrome)

start it like this:

```
$ chromium --proxy-server=127.0.0.1:8080
```

To launch a new instance of chrome from the command line 
there is no command line argument, however, there is a trick. 
A workaround is to pass the argument `--user-data-dir` 
(with no directory) to start a new session.

## Target Scope

Many times, we are expected to test only parts of an application,
and usually, the scope is limited by domain, subdomain. Burp gives a 
nice, simple-to-use interface to add, edit, and remove targets
from the scope.

Go to **Target -> Scope -> Add** for adding new scope.

We can always edit the URL in the URL editor window if we need to 
tweak it a bit orif we made any mistakes (just use **Edit** option).

When we paste the URL, we can choose the protocol, host/IP 
(with **regex**), port, and filename.

### Loading a list of targets from a file

Also we can load a list of targets from a file. This allows us to 
build a target list, which can simply be loaded into the **Scope** 
section by clicking on the **Load ...** button.

Once clicked, the File Browser dialog window opens and we can choose 
our file. The links need to be one on each line and based on their 
protocol, port number, path, and so on. For example:

```
http://example.com
http://example2.com:8080
http://hello.com/admin/
```

  * Almost all the tools follow the scope. 
  * The Proxy can be configured to only intercept in-scope items.
  * Only in-scope items for spidering or live scanning in the 
  scanner can be done.

### Exclude from scope

The target scope works on **URL patterns**. URL patterns can either 
be inclusive or exclusive. An inclusive pattern will allow all URLs 
matching the pattern to go through. An exclusive pattern will 
disallow all URLs matching the pattern from proceeding further.

Once a target URL pattern is matched, it is checked against in the 
**exclusion list**.

For example, if we want to attack everything and not get logged out, 
we can exclude the **Logout** page. If some functionality triggers 
automated e-mails to thousands of users, we don't want to annoy the 
users by sending e-mails while testing by mistake. We should 
explicitly put the mentioned URLs in the exclusion list.

### Setting up Burp to be a proxy server for other devices

What if we would like to intercept traffic from our television, 
from our iOS, or Android devices? Currently, in the default 
configuration, Burp has started one listener on an internal 
interface on port number **8080**. We can start multiple listeners on 
different ports and interfaces.

Go to **Proxy -> Options -> Add**

## SSL certificate setup

To intercept traffic over HTTPS, we need to configure some more 
settings. 

First of all, we should save(export) **BurpSuite** certificate.
Here you have two options:

  * Go to **Proxy -> Options -> Import / export CA certificate**
  * Or go to **localhost:8080** and click on **CA Certificate**

### Firefox

To import BurpSuite certificate open 

```
Preferences -> Advanced -> Certificates -> View Certificates -> Import
```

### Chromium

To import BurpSuite certificate open 

```
Settings -> Advanced -> Manage certificates -> Authorities -> Import
```

## Target

The **Target** tab has two subtabs for **Scope** and **Site map**.
The **Scope** subtab allows us to set the scope for our testing
and the **Site map** subtab shows us all requested and unrequested
items for the website. The **Site map** subtab automatically collects 
data from other sources, including Proxy, Spider, active and passive Scanner, etc.

Just as with any data aggregation tool, more power comes when we 
select data on various parameters, and Burp's **Display** filters do 
a great job here.

While mapping the application, it might make sense to uncheck 
**Hide not-found** items. This might point to some resource that was 
present earlier and give hints about the application and its flaws.

## Proxy

Inside Burp, type in `Ctrl + Shift + P` to easily switch to the 
**Proxy** tab and `Ctrl + F` to forward the waiting request.

To see all the keyboard shortcuts or edit them, go to 

```
Options -> Misc -> HotKeys
```

## Actions on the intercepted requests

Once we have an intercepted request, we can send it to Repeater 
(`Ctrl + R`) to manipulate it one by one; we can send it to Intruder 
(`Ctrl + I`) to simulate an automated attack; we can send it for more 
spidering, an active scan or even to a Sequencer, Decoder, 
or Comparer.

Repeater allows a tester to send requests and get instant feedback 
with the responses.

Intruder is like an iterator. We give it a range of values and 
unleash it on the application. In most cases, the output of basic 
test cases from Repeater will point you either to use Intruder or go 
with the active scanner.

Logical issues and input-based issues are better tested and verified 
with Repeater once and then exploited using Intruder.

## Intercept only one domain and subdomains

Burp’s scope rules (`Target -> Scope`) use regular expressions in the 
host field, so you 
can create a rule that matches something like:

```
.*\.google\.com$
```

and this should match all subdomains of `google.com`

After that you should go to 

```
Proxy -> Options -> Intercept Client Requests
```

and mark or create field **And | URL | Is in target scope**

## Using the Proxy history tab

Burp maintains all of Proxy history in the **HTTP history** subtab. 
In a lot of security assessments, allowing the site map to fill up 
and then going through the **HTTP history** table gives us a very 
good idea about the functionality, architecture,
and common patterns that might indicate vulnerabilities and more.

## Intruder

Burp Intruder is meant for exploitation and automating attacks.
Intruder is a very good and efficient request sender and response 
collector. 

The best way to get started is to find a request that has parameters 
that can be fuzzed. A login form is a good example where we can check 
for weak credentials by simulating a dictionary attack using the 
Intruder tool.

First, we choose an interesting-looking request that can and should 
be automated. 

Then we give a baseline request to Intruder (`Send to Intruder` or 
`Ctrl-I`), mark the positions where the payloads or attack test cases 
should be placed, and start the attack. 

## Emulate User Agent

Another useful trick is to match and replace the default user agent 
of the browser to emulate another browser or a mobile device:

Go to 

```
Proxy -> Options -> Match and Replace
```

## Grep - Match and Grep - Extract

We can use **Grep - Match** to quickly identify requests/responses 
that we get in the **Intruder** results to filter these results based 
on certain conditions.

We can add more keywords, set the match type to be simple strings or 
regular expression patterns, and load more from our list of keywords. 
It is great for analysis of output from the **Intruder** tool!

**Grep - Extract** allows us to extract data using the response 
extraction rules for the requests made in **Intruder**.





