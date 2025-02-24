#!/usr/bin/env ruby

headers = `git ls-files -o '*.h'`.lines.map {|line| line.strip}

headers.each do |header|
  original = '#include <' + header + '>'
  replacement = '#include "' + header + '"'
  sed_string = "s/#{original}/#{replacement}/g"
  `sed -i '#{sed_string}' *`
end
