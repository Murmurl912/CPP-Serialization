# CPP-Serialization
An XML approach to c++ object serialization

example serializable result:

<?xml version = "1.0" encoding = "utf-8"?>
<cs type="cats" version="1.0">
    <id type="int">123</id>
    <leader type="cat">
        <leader type="cat" version="1.0">
            <integer type="int">1</integer>
            <array type="int[]">
                <length type="int">10</length>
                <index_0 type="int">1</index_0>
                <index_1 type="int">2</index_1>
                <index_2 type="int">3</index_2>
                <index_3 type="int">4</index_3>
                <index_4 type="int">5</index_4>
                <index_5 type="int">6</index_5>
                <index_6 type="int">7</index_6>
                <index_7 type="int">8</index_7>
                <index_8 type="int">9</index_8>
                <index_9 type="int">10</index_9>
            </array>
        </leader>
    </leader>
</cs>
