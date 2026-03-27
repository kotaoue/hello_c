#!/usr/bin/env ruby
# CMock を使って clock.h のモックファイルを生成するスクリプト

$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'vendor', 'cmock', 'lib'))
require 'cmock'

cmock = CMock.new(
  mock_path: File.join(File.dirname(__FILE__), '..', 'mocks'),
  unity_helper_path: false
)

cmock.setup_mocks([File.join(File.dirname(__FILE__), '..', 'src', 'clock.h')])
puts "Mock generated successfully!"
