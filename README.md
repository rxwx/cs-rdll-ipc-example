## cs-rdll-example

This is an example code pattern for using named pipes for IPC with ReflectiveDlls in Cobalt Strike.

It is simply an example to show how to get beacon output from an injected rDLL. It does not do anything other than demonstrate how to send and receive output over a named pipe using aggressor and C++.

This is useful for scenarios where you want to inject a post-exploitation capability into the current (or remote) process to avoid the fork and inject method used by `bdllspawn`. Whilst `shinject` and `bdllinject` support self-injection (you can specify an arbitrary PID), they do not contain any built-in way to send back output to the current beacon. From Cobalt Strike 4.1, you probably want to use BOFs for this instead, however if for some reason that is not possible, this method can be used as an alternative.

## Note on Injection vs. Self-injection

The included CNA script is configured for self-injection, which is likely more stealthier in most scenarios. However, it does come with the risk that errors in your ReflectiveDLl will crash the current beacon. Therefore you will need to be mindful of this risk when using self-injection. The named pipe IPC technique does support remote injection though, you just need to specify the target PID in the CNA script instead of the current process.

## Running

Load the `pipetest.cna` script from the [cna/](cna/) folder. Once loaded, type `pipetest` in a beacon session. This will inject the example DLL into the current beacon process and print the output sent over the named pipe.