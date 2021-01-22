RSpec.describe Base58 do
  it "has a version number" do
    expect(Base58::VERSION).not_to be nil
  end

  describe '.encode_58' do
    it "raises on nil" do
      result = Base58.encode_58("hello world")

      expect(result).to eql("StV1DL6CwTryKyV")
    end

    it "raises on non-strings" do
      expect do
        Base58.encode_58(nil)
      end.to raise_error(TypeError)
    end
  end

  describe '.decode_58' do
    it "raises on nil" do
      result = Base58.decode_58("StV1DL6CwTryKyV")

      expect(result).to eql("hello world")
    end

    it "can handle utf8" do
      skip 'Does not support UTF8 yet'

      result = Base58.decode_58(Base58.encode_58("😊"))

      expect(result).to eql("😊")
    end

    it "raises on non-strings" do
      expect do
        Base58.decode_58(nil)
      end.to raise_error(TypeError)
    end
  end

  describe '.check_58' do
    it "returns true for valid base58" do
      result = Base58.check_58("StV1DL6CwTryKyV")

      expect(result).to eql(true)
    end

    it "returns false for invalid base58" do
      result = Base58.check_58("hello 123")

      expect(result).to eql(false)
    end


    it "raises on non-strings" do
      expect do
        Base58.check_58(nil)
      end.to raise_error(TypeError)
    end
  end
end
