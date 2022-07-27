## Language

English.

## Reporting Bugs

Bugs should be reported on the [GitHub Issue Tracker][issue-tracker] using the bug report template.

## Requesting New Features

Open an issue with the relevant labels.

## Contributing a Fix or Feature

Make sure your PR is from a forked branch that is up to date with master.

Go [Here][submit-pr] to submit a PR.

### Writing a Good Pull Request

- Stay focused on a single fix or feature.

- Limit your changes to only what is required to implement the fix or feature. In particular, avoid style or formatting tools that may modify the formatting of other areas of the code.

- Use descriptive commit titles/messages.

- Make sure the code you submit compiles and runs without issues.

- Follow coding conventions.

[issue-tracker]: https://github.com/domre/galaxy/issues
[submit-pr]: https://github.com/domre/galaxy/pulls

### TL;DR
Use github issues and pull requests.

Make sure your issues / pull requests decribe the problem you are having/trying to solve.

Use clang-format and some form of static code analysis before opening a pull request. CA configs are in repository root.

Open an issue that is correctly labelled and link it to your pull request, which is also correctly labelled.

Keep coding standard inline with modern `cmake` and `C++20` guidelines. Use `snake_case`, except for types and classes, which are `CamelCase`.