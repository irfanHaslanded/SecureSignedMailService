workflow "Unit Test" {
  on = "push"
  resolves = ["action-gtest"]
}

action "action-gtest" {
  uses = "CyberZHG/github-action-gtest@master"
  runs = "your command"
}
