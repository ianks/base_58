require "bundler/gem_tasks"
require "rspec/core/rake_task"

RSpec::Core::RakeTask.new(:spec)

require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new("base_58") do |ext|
  ext.lib_dir = "lib/base_58"
end

task :default => [:clobber, :compile, :spec]
